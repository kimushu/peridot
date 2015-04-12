#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <alt_types.h>
#include <io.h>
#include <sys/alt_cache.h>
#include <sys/alt_irq.h>

#include "mem_http.h"

#ifndef ALT_CPU_DCACHE_BYPASS_MASK
# define ALT_CPU_DCACHE_BYPASS_MASK (1u<<31)
#endif

static void rx_parse_method(mem_http_state *sp, mem_http_packet *pp);
static void rx_drop_packets(mem_http_state *sp, mem_http_packet *pp);
static void rx_recv_put_body(mem_http_state *sp, mem_http_packet *pp);

static void tx_send_get_header(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_get_content(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_no_content(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_bad_request(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_forbidden(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_not_found(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_int_srv_err(mem_http_state *sp, mem_http_packet *pp);
static void tx_send_not_impl(mem_http_state *sp, mem_http_packet *pp);

static int decode_uri(char *uri)
{
  char ch;
  char *write;

  for (;;) {
    ch = *uri;
    if (ch == 0) return 0;
    if (ch == '%') break;
    ++uri;
  }

  for (write = uri;; ++write) {
    ch = *uri++;
    if (ch != '%') {
      *write++ = ch;
      if (ch == 0) {
        break;
      }
    }

    ch = *uri++;
    ch |= (ch & 0x40) >> 1;
    if ('0' <= ch && ch <= '9') {
      *write = (ch - '0') << 4;
    }
    else if ('a' <= ch && ch <= 'f') {
      *write = (ch - 'a' + 10) << 4;
    }
    else {
      return -1;
    }

    ch = *uri++;
    ch |= (ch & 0x40) >> 1;
    if ('0' <= ch && ch <= '9') {
      *write |= (ch - '0');
    }
    else if ('a' <= ch && ch <= 'f') {
      *write |= (ch - 'a' + 10);
    }
    else {
      return -1;
    }
  }

  return 0;
}

static void rx_parse_method(mem_http_state *sp, mem_http_packet *pp)
{
  char *method;
  char *target;
  char *query;
  char *httpver;
  char *eol;
  char *body;
  char *host = NULL;
  char *content_length = NULL;
  char *last_modified = NULL;
  uint16_t flags = IORD_16DIRECT(&pp->flags, 0);

  sp->rx_action = flags ? NULL : rx_drop_packets;

  if (!(flags & MEM_HTTP_SOM)) {
    /* Must be start of message */
    ++sp->statistics.error.sequence;
    return;
  }

  /* Search splitter of request and message-body */
  body = memmem(pp->data, pp->length, "\r\n\r\n", 4);
  if (!body) {
    /* Header must be stored in one packet */
    goto bad_request;
  }

  /* Make request part null-terminated string */
  body[2] = 0;
  body += 4;

  /* request-line = method SP request-target SP HTTP-version CRLF */
  method = pp->data;
  target = strchr(method, ' ');
  if (!target) goto bad_request;
  *target++ = 0;
  httpver = strchr(target, ' ');
  if (*target != '/') goto bad_request;  /* accept abs_path only */
  if (!httpver || httpver == target) goto bad_request;
  *httpver++ = 0;
  query = strchr(target, '?');
  if (query) *query++ = 0;
  if (decode_uri(target) < 0) goto bad_request;
  eol = strstr(httpver, "\r\n");
  if (!eol) goto bad_request;
  *eol = 0;
  if (strcmp(httpver, "HTTP/1.0") != 0 &&
      strcmp(httpver, "HTTP/1.1") != 0) goto bad_request;

  /* *( header-field CRLF ) */
  while(1)
  {
    char *name;
    char *value;
    /* header-field = field-name ":" OWS field-value OWS */
    name = eol + 2;
    if (!name[0]) break;
    eol = strstr(name, "\r\n");
    if (!eol) goto bad_request;
    *eol = 0;
    value = strchr(name, ':');
    if (!value) goto bad_request;
    *value++ = 0;
    while(*value == ' ') ++value;

    if (strcmpi(name, "Host") == 0) {
      host = value;
    }
    else if (strcmpi(name, "Content-Length") == 0) {
      content_length = value;
    }
    else if (strcmpi(name, "Last-Modified") == 0) {
      last_modified = value;
    }
  }

  if (strcmp(method, "GET") == 0) {
    /* Read (download) file */
    ++sp->statistics.method.get;
    if ((sp->fd = open(target, O_RDONLY)) != -1) {
      struct stat st;
      if (fstat(sp->fd, &st) != -1) {
        sp->content_type = "application/octet-stream";
        sp->remainder = st.st_size;
        sp->tx_action = tx_send_get_header;
      }
      else {
        close(sp->fd);
        sp->fd = -1;
        sp->tx_action = tx_send_int_srv_err;
      }
    }
    else if (errno == EPERM) {
      sp->tx_action = tx_send_forbidden;
    }
    else {
      sp->tx_action = tx_send_not_found;
    }
  }
  else if (strcmp(method, "PUT") == 0) {
    /* Write (upload) file */
    ++sp->statistics.method.put;
    if (!content_length) goto bad_request;
    sp->remainder = strtoul(content_length, NULL, 10);
    if ((sp->fd = open(target, O_WRONLY | O_CREAT | O_TRUNC)) != -1) {
      sp->rx_action = rx_recv_put_body;
    }
    else if (errno == EPERM) {
      sp->tx_action = tx_send_forbidden;
    }
    else {
      sp->tx_action = tx_send_int_srv_err;
    }
  }
  else if (strcmp(method, "DELETE") == 0) {
    /* Delete file */
    ++sp->statistics.method.delete;
    if (unlink(target) != -1) {
      sp->tx_action = tx_send_no_content;
    }
    else if (errno == EPERM) {
      sp->tx_action = tx_send_forbidden;
    }
    else {
      sp->tx_action = tx_send_int_srv_err;
    }
  }
  else if (strcmp(method, "PROPFIND") == 0) {
    /* Enumerate file and/or properties */
    // TODO
    ++sp->statistics.method.propfind;
    sp->tx_action = tx_send_not_impl;
  }
  else if (strcmp(method, "POST") == 0) {
    /* Special commands */
    // TODO
    ++sp->statistics.method.post;
    switch(sp->post_receiver ? sp->post_receiver(target) : 403)
    {
    case 204:
      sp->tx_action = tx_send_no_content;
      break;
    case 403:
      sp->tx_action = tx_send_forbidden;
      break;
    default:
      sp->tx_action = tx_send_int_srv_err;
      break;
    }
  }
  else {
    /* Unknown method */
bad_request:
    sp->tx_action = tx_send_bad_request;
  }

  if (sp->rx_action && body) {
    size_t body_length;

    /* process body */
    body_length = pp->length - (body - pp->data);
    memmove(pp->data, body, body_length);
    pp->length = body_length;
    (*sp->rx_action)(sp, pp);
  }
}

static void rx_drop_packets(mem_http_state *sp, mem_http_packet *pp)
{
  if (IORD_16DIRECT(&pp->flags, 0) & MEM_HTTP_EOM) {
    sp->rx_action = NULL;
  }
}

static void rx_recv_put_body(mem_http_state *sp, mem_http_packet *pp)
{
  size_t length = pp->length;
  uint16_t flags = IORD_16DIRECT(&pp->flags, 0);

  if (sp->remainder < length) {
    length = sp->remainder;
  }

  if (length == 0 || write(sp->fd, pp->data, length) == length) {
    sp->remainder -= length;
    if (sp->remainder > 0 && !(flags & MEM_HTTP_EOM)) {
      return;
    }
    sp->tx_action = tx_send_no_content;
  }
  else {
    sp->tx_action = tx_send_int_srv_err;
  }

  close(sp->fd);
  sp->fd = -1;
  sp->rx_action = (flags & MEM_HTTP_EOM) ? NULL : rx_drop_packets;
}

static void do_response(mem_http_state *sp, mem_http_packet *pp,
                        const char *data,
                        void (*action)(mem_http_state *, mem_http_packet *))
{
  if (data) {
    strcpy(pp->data, data);
  }
  pp->length = strlen(pp->data);
  IOWR_16DIRECT(&pp->flags, 0, MEM_HTTP_SOM | MEM_HTTP_VALID | (action ? 0 : MEM_HTTP_EOM));
  sp->tx_action = action;
}

static void tx_send_get_header(mem_http_state *sp, mem_http_packet *pp)
{
  sprintf(pp->data,
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: %s\r\n"
      "Content-Length: %u\r\n\r\n",
      sp->content_type,
      sp->remainder);

  do_response(sp, pp, NULL, (sp->remainder > 0) ? tx_send_get_content : NULL);
}

static void tx_send_get_content(mem_http_state *sp, mem_http_packet *pp)
{
  size_t length = pp->capacity;

  if (sp->remainder < length) {
    length = sp->remainder;
  }

  if (length == 0 || read(sp->fd, pp->data, length) == length) {
    sp->remainder -= length;
    pp->length = length;
    if (sp->remainder > 0) {
      pp->flags = 0;
      goto flush_and_validate;
    }
    ++sp->statistics.success.ok;
  }
  else {
    /* read error => abort response */
    pp->length = 0;
    ++sp->statistics.error.abort;
  }

  close(sp->fd);
  sp->fd = -1;
  sp->tx_action = NULL;
  pp->flags = MEM_HTTP_EOM;

flush_and_validate:
  alt_dcache_flush(pp, sizeof(*pp) + pp->length);
  IOWR_16DIRECT(&pp->flags, 0, pp->flags | MEM_HTTP_VALID);
}

static void tx_send_no_content(mem_http_state *sp, mem_http_packet *pp)
{
  do_response(sp, pp, "HTTP/1.1 204 No Content\r\n\r\n", NULL);
  ++sp->statistics.success.no_content;
}

static void tx_send_bad_request(mem_http_state *sp, mem_http_packet *pp)
{
  do_response(sp, pp, "HTTP/1.1 400 Bad Request\r\n\r\n", NULL);
  ++sp->statistics.error.bad_request;
}

static void tx_send_forbidden(mem_http_state *sp, mem_http_packet *pp)
{
  do_response(sp, pp, "HTTP/1.1 403 Forbidden\r\n\r\n", NULL);
  ++sp->statistics.error.forbidden;
}

static void tx_send_not_found(mem_http_state *sp, mem_http_packet *pp)
{
  do_response(sp, pp, "HTTP/1.1 404 Not Found\r\n\r\n", NULL);
  ++sp->statistics.error.not_found;
}

static void tx_send_int_srv_err(mem_http_state *sp, mem_http_packet *pp)
{
  do_response(sp, pp, "HTTP/1.1 500 Internal Server Error\r\n\r\n", NULL);
  ++sp->statistics.error.int_srv_err;
}

static void tx_send_not_impl(mem_http_state *sp, mem_http_packet *pp)
{
  do_response(sp, pp, "HTTP/1.1 501 Not Implemented\r\n\r\n", NULL);
  ++sp->statistics.error.not_impl;
}

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void mem_http_irq(void *context)
#else
static void mem_http_irq(void *context, alt_u32 id)
#endif
{
  mem_http_state *sp = (mem_http_state *)context;
  mem_http_packet *pp;
  int retry;

  do {
    retry = 0;

    if (sp->buffer->irq_base) {
      IOWR(sp->buffer->irq_base, 1, 1); /* clear interrupt */
    }

    if (!sp->rx_action && !sp->tx_action) {
      sp->rx_action = rx_parse_method;
    }

    if (sp->rx_action) {
      pp = sp->rx_read;
      if (IORD_16DIRECT(&pp->flags, 0) & MEM_HTTP_VALID) {
        /* process received packet */
#if (ALT_CPU_DCACHE_SIZE > 0)
        alt_dcache_flush_no_writeback(pp, sizeof(*pp) + IORD_16DIRECT(&pp->length, 0));
#endif
        (*sp->rx_action)(sp, pp);

        /* mark this packet as empty */
        IOWR_16DIRECT(&pp->flags, 0, 0);

        sp->rx_read = (mem_http_packet *)((char *)sp->buffer + pp->next);
        retry = 1;
      }
    }
    else if (sp->tx_action) {
      pp = sp->tx_write;
      if (!(IORD_16DIRECT(&pp->flags, 0) & MEM_HTTP_VALID)) {
        /* send next packet */
        (*sp->tx_action)(sp, pp);
        sp->tx_write = (mem_http_packet *)((char *)sp->buffer + pp->next);
        retry = 1;
      }
    }
  } while (retry);
}

void mem_http_init(mem_http_state *sp, void *irq_base, int irq_controller_id, int irq,
                   size_t rx_size, size_t rx_count, size_t tx_size, size_t tx_count,
                   int (*post_receiver)(const char *))
{
  uint16_t next;
  size_t count;
  mem_http_packet *header;

  /* initialize state */
  sp->rx_action = NULL;
  sp->tx_action = NULL;
  sp->post_receiver = post_receiver;

  /* initialize buffer */
  sp->buffer->signature[0] = 0;
  sp->buffer->irq_base = irq_base;
  next = sizeof(*sp->buffer);
  header = (mem_http_packet *)((char *)sp->buffer + next);

  /* setup RX buffers */
  sp->buffer->rx_write = next;
  sp->rx_read = header;
  for (count = rx_count; count > 0; --count) {
    header->capacity = rx_size - sizeof(*header);
    header->flags = 0;
    header->length = 0;
    next += rx_size;
    header->next = (count > 1) ? next : sp->buffer->rx_write;
    alt_dcache_flush(header, sizeof(*header));
    header = (mem_http_packet *)((char *)sp->buffer + next);
  }

  /* setup TX buffers */
  sp->buffer->tx_read = next;
  sp->tx_write = header;
  for (count = tx_count; count > 0; --count) {
    header->capacity = tx_size - sizeof(*header);
    header->flags = 0;
    header->length = 0;
    next += tx_size;
    header->next = (count > 1) ? next : sp->buffer->tx_read;
    alt_dcache_flush(header, sizeof(*header));
    header = (mem_http_packet *)((char *)sp->buffer + next);
  }

  /* buffer setup done */
  memcpy(sp->buffer->signature, MEM_HTTP_SIGNATURE, sizeof(sp->buffer->signature));
  alt_dcache_flush(sp->buffer, sizeof(*sp->buffer));

  /* register interrupt */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
  alt_ic_isr_register(irq_controller_id, irq, mem_http_irq, sp, NULL);
#else
  alt_irq_register(irq, sp, mem_http_irq);
#endif
}

/* vim: set et sts=2 sw=2: */
