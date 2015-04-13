#include <alt_types.h>
#include <unistd.h>
#include <string.h>
#include "system.h"

#define MRB_WORD_BOXING   /* Do not forget define this */
#include "mruby.h"
#include "mruby/dump.h"
#include "mruby/opcode.h"
#include "mruby/variable.h"

#include "rubic.h"
#include "mem_http.h"
#include "simple_epcsfs.h"
#include "io.h"

rubic_state rubic;
MEM_HTTP_STATE_INSTANCE(RAM_HTTP, mem_http);

SIMPLE_EPCSFS_DEV_INSTANCE(EPCS, epcs);

/*
 * Altera's HAL does not have sleep() function
 * This supplements sleep() by using usleep(500000)x2
 */
unsigned int sleep(unsigned int seconds)
{
  for(; seconds > 0; --seconds) {
    usleep(500000);
    usleep(500000);
  }
  return 0;
}

/* Peridot.start_led */
static mrb_value
peridot_start_led(mrb_state *mrb, mrb_value self)
{
  return mrb_cv_get(mrb, self, mrb_intern_lit(mrb, "@@pio_led"));
}

/* Peridot.digital_io */
static mrb_value
peridot_digital_io(mrb_state *mrb, mrb_value self)
{
  return mrb_cv_get(mrb, self, mrb_intern_lit(mrb, "@@pio_gpio"));
}

/* Initializer for Peridot class */
void
peridot_class_init(mrb_state *mrb, struct RClass *cls)
{
  mrb_value mod_alt;
  mrb_value cls_pio;

  mod_alt = mrb_const_get(mrb, mrb_obj_value(mrb->object_class), mrb_intern_lit(mrb, "Altera"));
  cls_pio = mrb_const_get(mrb, mod_alt, mrb_intern_lit(mrb, "PIOCore"));

  mrb_mod_cv_set(mrb, cls, mrb_intern_lit(mrb, "@@pio_led"),
      mrb_funcall(mrb, cls_pio, "new", 2,
          mrb_fixnum_value(PIO_LED_BASE), mrb_fixnum_value(PIO_LED_DATA_WIDTH)));
  mrb_define_class_method(mrb, cls, "start_led" , peridot_start_led , MRB_ARGS_NONE());

  mrb_mod_cv_set(mrb, cls, mrb_intern_lit(mrb, "@@pio_gpio"),
      mrb_funcall(mrb, cls_pio, "new", 2,
          mrb_fixnum_value(PIO_GPIO_BASE), mrb_fixnum_value(PIO_GPIO_DATA_WIDTH)));
  mrb_define_class_method(mrb, cls, "digital_io", peridot_digital_io, MRB_ARGS_NONE());
}

/* Finalizer for Peridot class */
void
peridot_class_final(mrb_state *mrb)
{
  /* do nothing */
}

/* POST method handler */
int
rubic_post(const char *target)
{
  if (strcmp(target, "/start") == 0)
  {
    if (rubic.state == RUBIC_STATE_STOPPED) {
      rubic.state = RUBIC_STATE_READY;
      return 204;
    }
  }
  else if (strcmp(target, "/stop") == 0)
  {
    if (rubic.state == RUBIC_STATE_RUNNING) {
      rubic.state = RUBIC_STATE_ABORTING;
      return 204;
    }
  }
  return 403;
}

mrb_code rubic_code_fetch_hook(mrb_state *mrb, mrb_irep *irep, mrb_code *pc, mrb_value *regs)
{
  if (rubic.state == RUBIC_STATE_ABORTING) {
    return MKOPCODE(OP_STOP);
  }
  return *pc;
}

int main()
{
  FILE *fp;

  MEM_HTTP_STATE_INIT(RAM_HTTP, mem_http, IRQ_SENDER, 256, 5, 256, 2, rubic_post);
  SIMPLE_EPCSFS_DEV_INIT(EPCS, epcs);

  /* TODO: safe mode */
  rubic.state = RUBIC_STATE_READY;

  while (1)
  {
    if (rubic.state != RUBIC_STATE_READY)
    {
      rubic.state = RUBIC_STATE_STOPPED;
      while (rubic.state != RUBIC_STATE_READY);
    }

    rubic.state = RUBIC_STATE_LOADING;
    fp = fopen("/mnt/epcs/main.mrb", "rb");
    if (fp == NULL)
    {
      continue;
    }

    rubic.mrb = mrb_open();
    rubic.state = RUBIC_STATE_RUNNING;

    mrb_load_irep_file_cxt(rubic.mrb, fp, NULL);
    //mrb_load_irep_cxt(rubic.mrb, main_mrb, NULL);

    mrb_close(rubic.mrb);
    rubic.state = RUBIC_STATE_STOPPED;
    fclose(fp);
  }
}

/* vim: set et sts=2 sw=2: */
