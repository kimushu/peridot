/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_core0' in SOPC Builder design 'plain'
 * SOPC Builder design path: ../../plain.sopcinfo
 *
 * Generated: Tue Apr 14 01:06:11 JST 2015
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x0fff0820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1d
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x00000020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 8192
#define ALT_CPU_INST_ADDR_WIDTH 0x1c
#define ALT_CPU_NAME "nios2_core0"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x0fff0000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x0fff0820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1d
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x00000020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 8192
#define NIOS2_INST_ADDR_WIDTH 0x1c
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x0fff0000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_EPCS_FLASH_CONTROLLER
#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_GEN2
#define __AVM_IRQ_SENDER


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone IV E"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0xfff1000
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0xfff1000
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0xfff1000
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "plain"


/*
 * epcs configuration
 *
 */

#define ALT_MODULE_CLASS_epcs altera_avalon_epcs_flash_controller
#define EPCS_BASE 0xfff0000
#define EPCS_IRQ 0
#define EPCS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define EPCS_NAME "/dev/epcs"
#define EPCS_REGISTER_OFFSET 1024
#define EPCS_SPAN 2048
#define EPCS_TYPE "altera_avalon_epcs_flash_controller"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 8
#define ALT_SYS_CLK TIMER_TICK
#define ALT_TIMESTAMP_CLK none


/*
 * irq_sender configuration
 *
 */

#define ALT_MODULE_CLASS_irq_sender avm_irq_sender
#define IRQ_SENDER_BASE 0xfff2000
#define IRQ_SENDER_IRQ 2
#define IRQ_SENDER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define IRQ_SENDER_NAME "/dev/irq_sender"
#define IRQ_SENDER_SPAN 8
#define IRQ_SENDER_TYPE "avm_irq_sender"


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0xfff1000
#define JTAG_UART_IRQ 1
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * pio_gpio configuration
 *
 */

#define ALT_MODULE_CLASS_pio_gpio altera_avalon_pio
#define PIO_GPIO_BASE 0x10000220
#define PIO_GPIO_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_GPIO_BIT_MODIFYING_OUTPUT_REGISTER 1
#define PIO_GPIO_CAPTURE 0
#define PIO_GPIO_DATA_WIDTH 28
#define PIO_GPIO_DO_TEST_BENCH_WIRING 0
#define PIO_GPIO_DRIVEN_SIM_VALUE 0
#define PIO_GPIO_EDGE_TYPE "NONE"
#define PIO_GPIO_FREQ 40000000
#define PIO_GPIO_HAS_IN 0
#define PIO_GPIO_HAS_OUT 0
#define PIO_GPIO_HAS_TRI 1
#define PIO_GPIO_IRQ -1
#define PIO_GPIO_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_GPIO_IRQ_TYPE "NONE"
#define PIO_GPIO_NAME "/dev/pio_gpio"
#define PIO_GPIO_RESET_VALUE 0
#define PIO_GPIO_SPAN 32
#define PIO_GPIO_TYPE "altera_avalon_pio"


/*
 * pio_led configuration
 *
 */

#define ALT_MODULE_CLASS_pio_led altera_avalon_pio
#define PIO_LED_BASE 0x10000200
#define PIO_LED_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_LED_BIT_MODIFYING_OUTPUT_REGISTER 1
#define PIO_LED_CAPTURE 0
#define PIO_LED_DATA_WIDTH 1
#define PIO_LED_DO_TEST_BENCH_WIRING 0
#define PIO_LED_DRIVEN_SIM_VALUE 0
#define PIO_LED_EDGE_TYPE "NONE"
#define PIO_LED_FREQ 40000000
#define PIO_LED_HAS_IN 0
#define PIO_LED_HAS_OUT 1
#define PIO_LED_HAS_TRI 0
#define PIO_LED_IRQ -1
#define PIO_LED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_LED_IRQ_TYPE "NONE"
#define PIO_LED_NAME "/dev/pio_led"
#define PIO_LED_RESET_VALUE 0
#define PIO_LED_SPAN 32
#define PIO_LED_TYPE "altera_avalon_pio"


/*
 * ram_http configuration
 *
 */

#define ALT_MODULE_CLASS_ram_http altera_avalon_onchip_memory2
#define RAM_HTTP_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define RAM_HTTP_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define RAM_HTTP_BASE 0xfffc000
#define RAM_HTTP_CONTENTS_INFO ""
#define RAM_HTTP_DUAL_PORT 1
#define RAM_HTTP_GUI_RAM_BLOCK_TYPE "AUTO"
#define RAM_HTTP_INIT_CONTENTS_FILE "plain_ram_http"
#define RAM_HTTP_INIT_MEM_CONTENT 0
#define RAM_HTTP_INSTANCE_ID "NONE"
#define RAM_HTTP_IRQ -1
#define RAM_HTTP_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RAM_HTTP_NAME "/dev/ram_http"
#define RAM_HTTP_NON_DEFAULT_INIT_FILE_ENABLED 0
#define RAM_HTTP_RAM_BLOCK_TYPE "AUTO"
#define RAM_HTTP_READ_DURING_WRITE_MODE "DONT_CARE"
#define RAM_HTTP_SINGLE_CLOCK_OP 0
#define RAM_HTTP_SIZE_MULTIPLE 1
#define RAM_HTTP_SIZE_VALUE 2048
#define RAM_HTTP_SPAN 2048
#define RAM_HTTP_TYPE "altera_avalon_onchip_memory2"
#define RAM_HTTP_WRITABLE 1


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x0
#define SDRAM_CAS_LATENCY 2
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 8
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 200.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x16
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 8
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 12
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 0
#define SDRAM_SPAN 8388608
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 6.0
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 67.5
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 15.0


/*
 * sysid configuration
 *
 */

#define ALT_MODULE_CLASS_sysid altera_avalon_sysid_qsys
#define SYSID_BASE 0x10000000
#define SYSID_ID 538185769
#define SYSID_IRQ -1
#define SYSID_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_NAME "/dev/sysid"
#define SYSID_SPAN 8
#define SYSID_TIMESTAMP 1428939852
#define SYSID_TYPE "altera_avalon_sysid_qsys"


/*
 * timer_tick configuration
 *
 */

#define ALT_MODULE_CLASS_timer_tick altera_avalon_timer
#define TIMER_TICK_ALWAYS_RUN 0
#define TIMER_TICK_BASE 0x10000100
#define TIMER_TICK_COUNTER_SIZE 32
#define TIMER_TICK_FIXED_PERIOD 0
#define TIMER_TICK_FREQ 40000000
#define TIMER_TICK_IRQ 3
#define TIMER_TICK_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_TICK_LOAD_VALUE 39999
#define TIMER_TICK_MULT 0.001
#define TIMER_TICK_NAME "/dev/timer_tick"
#define TIMER_TICK_PERIOD 1
#define TIMER_TICK_PERIOD_UNITS "ms"
#define TIMER_TICK_RESET_OUTPUT 0
#define TIMER_TICK_SNAPSHOT 1
#define TIMER_TICK_SPAN 32
#define TIMER_TICK_TICKS_PER_SEC 1000
#define TIMER_TICK_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TICK_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
