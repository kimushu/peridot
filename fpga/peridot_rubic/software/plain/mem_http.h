#ifndef __MEM_HTTP_H_
#define __MEM_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MEM_HTTP_SIGNATURE  "MHttp1.0"

#define MEM_HTTP_VALID      0x0001
#define MEM_HTTP_SOM        0x0002
#define MEM_HTTP_EOM        0x0004

#ifdef MEM_HTTP_DETAIL_STATISTICS
# define STAT_TYPE  struct
#else
# define STAT_TYPE  union
#endif

typedef struct mem_http_buffer_s {
  char signature[8];
  void *irq_base;
  uint16_t rx_write;
  uint16_t tx_read;
  uint8_t reserved[16];
} mem_http_buffer;

typedef struct mem_http_packet_s {
  uint16_t next;
  uint16_t capacity;
  uint16_t flags;
  uint16_t length;
  char data[0];
} mem_http_packet;

typedef struct mem_http_state_s {
  mem_http_buffer *buffer;

  mem_http_packet *rx_read;
  mem_http_packet *tx_write;
  void (*rx_action)(struct mem_http_state_s *, mem_http_packet *);
  void (*tx_action)(struct mem_http_state_s *, mem_http_packet *);

  int (*post_receiver)(const char *target);

  int fd;
  size_t remainder;
  const char *content_type;

  struct {
    STAT_TYPE {
      int get;
      int put;
      int delete;
      int propfind;
      int post;
    } method;
    STAT_TYPE {
      int ok;           /* 200 */
      int no_content;   /* 204 */
    } success;
    STAT_TYPE {
      int bad_request;  /* 400 */
      int forbidden;    /* 403 */
      int not_found;    /* 404 */
      int int_srv_err;  /* 500 */
      int not_impl;     /* 501 */
      int sequence;
      int abort;
    } error;
  } statistics;
} mem_http_state;

#undef STAT_TYPE

#define MEM_HTTP_STATE_INSTANCE(name, state) \
  mem_http_state state = { (mem_http_buffer *)(name##_BASE) }

extern void mem_http_init(mem_http_state *sp, void *irq_base, int irq_controller_id, int irq,
                          size_t rx_size, size_t rx_count, size_t tx_size, size_t tx_count,
                          int (*post_receiver)(const char *));

#define MEM_HTTP_STATE_INIT(name, state, irqname, rxs, rxc, txs, txc, post) \
  do { \
    __attribute__((unused)) extern char \
        __buffers_for_mem_http_exceeds_capacity_of_ram__[ \
        (((rxs)+sizeof(mem_http_packet))*(rxc)+ \
         ((txs)+sizeof(mem_http_packet))*(txc)+ \
         sizeof(mem_http_buffer)) <= name##_SPAN ? 1 : -1]; \
    mem_http_init(&state, (void *)irqname##_BASE, \
        irqname##_IRQ_INTERRUPT_CONTROLLER_ID, irqname##_IRQ, \
        (rxs), (rxc), (txs), (txc), (post)); \
  } while (0)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* !__MEM_HTTP_H_ */
/* vim: set et sts=2 sw=2: */
