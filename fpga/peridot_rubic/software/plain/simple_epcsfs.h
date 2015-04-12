#ifndef __SIMPLE_EPCSFS_H_
#define __SIMPLE_EPCSFS_H_

#include <stdint.h>
#include "sys/alt_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EPCS_CAPACITY     (512*1024)
#define EPCS_OFFSET_BYTES (EPCS_CAPACITY-64*1024)

#define EPCS_SECTOR_SIZE  4096
#define EPCS_SECTOR_MASK  (EPCS_SECTOR_SIZE-1)
#define EPCS_PAGE_SIZE    256

typedef struct simple_epcsfs_state_s
{
  uint32_t base;
  int flags;
  uint32_t eof_prev;
  uint32_t eof_ptr;
  uint32_t rw_ptr;
  uint32_t buf_ptr;
  uint8_t buffer[EPCS_SECTOR_SIZE];
} simple_epcsfs_state;

typedef struct simple_epcsfs_dev_s
{
  alt_dev dev;
  simple_epcsfs_state state;
} simple_epcsfs_dev;

extern void simple_epcsfs_init(simple_epcsfs_state *sp);

extern int simple_epcsfs_open(alt_fd *fd, const char *name, int flags, int mode);
extern int simple_epcsfs_close(alt_fd *fd);
extern int simple_epcsfs_read(alt_fd *fd, char *ptr, int len);
extern int simple_epcsfs_write(alt_fd *fd, const char *ptr, int len);
extern int simple_epcsfs_lseek(alt_fd *fd, int ptr, int dir);

#define SIMPLE_EPCSFS_DEV_INSTANCE(name, d) \
  static simple_epcsfs_dev d =              \
  {                                         \
    {                                       \
      ALT_LLIST_ENTRY,                      \
      "/mnt/epcs",                          \
      simple_epcsfs_open,                   \
      simple_epcsfs_close,                  \
      simple_epcsfs_read,                   \
      simple_epcsfs_write,                  \
      simple_epcsfs_lseek,                  \
      NULL /* simple_epcsfs_fstat */,       \
      NULL /* simple_epcsfs_ioctl */,       \
    },                                      \
    {                                       \
      (name##_BASE)+(name##_REGISTER_OFFSET)\
    },                                      \
  }

#define SIMPLE_EPCSFS_DEV_INIT(name, d) \
  {                                     \
    simple_epcsfs_init(&d.state);       \
    alt_fs_reg(&d.dev);                 \
  }

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* !__SIMPLE_EPCSFS_H_ */
/* vim: set et sts=2 sw=2: */
