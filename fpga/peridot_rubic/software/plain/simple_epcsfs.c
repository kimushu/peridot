#include <string.h>
#include <errno.h>
#include "sys/alt_dev.h"
#include "altera_avalon_spi.h"
#include "epcs_commands.h"
#include "simple_epcsfs.h"
#include <unistd.h>
#include <fcntl.h>

typedef struct simple_epcsfs_file_s
{
  uint32_t size;  /* if file does not exist, set 0xffffffff here */
} simple_epcsfs_file;

void simple_epcsfs_init(simple_epcsfs_state *sp)
{
  /* do nothing */
  sp->eof_ptr = -1;
}

static void epcs_small_sector_erase(alt_u32 base, alt_u32 offset)
{
  alt_u8 se[4];
  se[0] = 0x20;
  se[1] = (offset >> 16) & 0xff;
  se[2] = (offset >>  8) & 0xff;
  se[3] = (offset >>  0) & 0xff;

  epcs_write_enable(base);

  alt_avalon_spi_command(
      base,
      0,
      sizeof(se),
      se,
      0,
      NULL,
      0);

  while(epcs_read_status_register(base) & 1);
}

static int simple_epcsfs_flush(simple_epcsfs_state *sp, int force)
{
  uint32_t page_offset;

  if (sp->buf_ptr != (uint32_t)-1) {
    if (!force && (sp->rw_ptr & EPCS_SECTOR_MASK) == (sp->buf_ptr & EPCS_SECTOR_MASK)) {
      /* no sector change */
      return 0;
    }

    if (sp->buf_ptr & 1) {
      /* write back */
      sp->buf_ptr &= ~EPCS_SECTOR_MASK;
      epcs_small_sector_erase(sp->base, EPCS_OFFSET_BYTES + EPCS_SECTOR_SIZE + sp->buf_ptr);

      for(page_offset = 0;
          page_offset < EPCS_SECTOR_SIZE;
          page_offset += EPCS_PAGE_SIZE) {
        epcs_write_buffer(
            sp->base,
            EPCS_OFFSET_BYTES + EPCS_SECTOR_SIZE + sp->buf_ptr + page_offset,
            sp->buffer + page_offset,
            EPCS_PAGE_SIZE,
            0);
      }
    }
    sp->buf_ptr = -1;
  }

  if (sp->rw_ptr == (uint32_t)-1) {
    return 0;
  }

  sp->buf_ptr = sp->rw_ptr & ~EPCS_SECTOR_MASK;
  epcs_read_buffer(
      sp->base,
      EPCS_OFFSET_BYTES + EPCS_SECTOR_SIZE + sp->buf_ptr,
      sp->buffer,
      EPCS_SECTOR_SIZE,
      0);
  return 0;
}

int simple_epcsfs_open(alt_fd *fd, const char *name, int flags, int mode)
{
  simple_epcsfs_dev *dev = (simple_epcsfs_dev *)fd->dev;
  simple_epcsfs_state *sp = (simple_epcsfs_state *)&dev->state;

  name += strlen(dev->dev.name) + 1;
  if (strcmp(name, "main.mrb") != 0) {
    return -ENOENT;
  }

  if (sp->eof_ptr != (uint32_t)-1) {
    return -EMFILE;
  }

  sp->flags = (flags & O_ACCMODE) + 1;

  epcs_read_buffer(sp->base, EPCS_OFFSET_BYTES, (uint8_t *)&sp->eof_prev, sizeof(sp->eof_prev), 0);
  if (sp->eof_prev == (uint32_t)-1) {
    /* new file */
    if (!(flags & O_CREAT)) {
      return -ENOENT;
    }
    sp->eof_ptr = 0;
  }
  else if ((flags & O_TRUNC) && (sp->flags & _FWRITE)) {
    sp->eof_ptr = 0;
  }
  else {
    sp->eof_ptr = sp->eof_prev;
  }
  sp->rw_ptr = 0;
  sp->buf_ptr = -1;
  return 0;
}

int simple_epcsfs_close(alt_fd *fd)
{
  simple_epcsfs_dev *dev = (simple_epcsfs_dev *)fd->dev;
  simple_epcsfs_state *sp = (simple_epcsfs_state *)&dev->state;

  if (sp->eof_ptr == (uint32_t)-1) {
    return -EBADF;
  }

  sp->rw_ptr = -1;
  simple_epcsfs_flush(sp, 1);
  if ((sp->eof_ptr != sp->eof_prev) && (sp->flags & _FWRITE)) {
    epcs_small_sector_erase(sp->base, EPCS_OFFSET_BYTES);
    epcs_write_buffer(sp->base, EPCS_OFFSET_BYTES, (uint8_t *)&sp->eof_ptr, sizeof(sp->eof_ptr), 0);
  }
  return 0;
}

int simple_epcsfs_read(alt_fd *fd, char *ptr, int len)
{
  simple_epcsfs_dev *dev = (simple_epcsfs_dev *)fd->dev;
  simple_epcsfs_state *sp = (simple_epcsfs_state *)&dev->state;
  int read_len, remainder;

  if (sp->eof_ptr == (uint32_t)-1) {
    return -EBADF;
  }
  if (!(sp->flags & _FREAD)) {
    return -EPERM;
  }

  read_len = sp->eof_ptr - sp->rw_ptr;
  if (read_len < len) {
    len = read_len;
  }

  for (remainder = len; remainder > 0;) {
    simple_epcsfs_flush(sp, 0);

    read_len = EPCS_SECTOR_SIZE - (sp->rw_ptr & EPCS_SECTOR_MASK);
    if (remainder < read_len) {
      read_len = remainder;
    }

    memcpy(ptr, sp->buffer + (sp->rw_ptr & EPCS_SECTOR_MASK), read_len);
    sp->rw_ptr += read_len;
    remainder -= read_len;
  }

  return len;
}

int simple_epcsfs_write(alt_fd *fd, const char *ptr, int len)
{
  simple_epcsfs_dev *dev = (simple_epcsfs_dev *)fd->dev;
  simple_epcsfs_state *sp = (simple_epcsfs_state *)&dev->state;
  int write_len, remainder;

  if (sp->eof_ptr == (uint32_t)-1) {
    return -EBADF;
  }
  if (!(sp->flags & _FWRITE)) {
    return -EPERM;
  }

  write_len = EPCS_CAPACITY - (EPCS_OFFSET_BYTES + EPCS_SECTOR_SIZE + sp->rw_ptr);
  if (write_len < len) {
    len = write_len;
  }

  for (remainder = len; remainder > 0;) {
    simple_epcsfs_flush(sp, 0);

    write_len = EPCS_SECTOR_SIZE - (sp->rw_ptr & EPCS_SECTOR_MASK);
    if (remainder < write_len) {
      write_len = remainder;
    }

    memcpy(sp->buffer + (sp->rw_ptr & EPCS_SECTOR_MASK), ptr, write_len);
    sp->rw_ptr += write_len;
    remainder -= write_len;

    sp->buf_ptr |= 1;
    if (sp->rw_ptr > sp->eof_ptr) {
      sp->eof_ptr = sp->rw_ptr;
    }
  }

  return len;
}

int simple_epcsfs_lseek(alt_fd *fd, int ptr, int dir)
{
  simple_epcsfs_dev *dev = (simple_epcsfs_dev *)fd->dev;
  simple_epcsfs_state *sp = (simple_epcsfs_state *)&dev->state;

  if (sp->eof_ptr == (uint32_t)-1) {
    return -EBADF;
  }

  switch(dir)
  {
  case SEEK_SET:
    sp->rw_ptr = dir;
    break;
  case SEEK_CUR:
    sp->rw_ptr += dir;
    break;
  case SEEK_END:
    sp->rw_ptr = sp->eof_ptr + dir;
    break;
  default:
    return -EINVAL;
  }

  return 0;
}

/* vim: set et sts=2 sw=2: */
