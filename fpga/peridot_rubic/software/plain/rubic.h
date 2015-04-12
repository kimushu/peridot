#ifndef __RUBIC_H_
#define __RUBIC_H_

#ifdef __cplusplus
extern "C" {
#endif

enum
{
  RUBIC_STATE_STOPPED,
  RUBIC_STATE_READY,
  RUBIC_STATE_LOADING,
  RUBIC_STATE_RUNNING,
  RUBIC_STATE_ABORTING,
};

typedef struct rubic_state_s
{
  volatile int state;
  mrb_state *mrb;
} rubic_state;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* !__RUBIC_H_ */
/* vim: set et sts=2 sw=2: */
