################ Top configuration.
PRJ_TOP = ..
TARGET = templ
OBJDIR = obj

OPTI = -O1

VER_STRING := $$(date +%Y%m%d_%H%M%S)_$(OPTI)

BUILD_STRICT = FALSE
BUILD_PRINT = FALSE

LINK_SCRIPT = startup/STM32F103C8TX_FLASH.ld
#LINK_SCRIPT += startup/mem.ld

################  Define
DEFINE = 
# STM32F103C8T6 <-- MD performance.
DEFINE += -DSTM32F103xB
DEFINE += -DUSE_HAL_DRIVER
DEFINE += -DDEBUG
#DEFINE += -DHSE_VALUE=8000000

################  Include.
# Add relative path from $(PRJ_TOP)
PRJ_INC = \
		mcu \
		hal \
		app

# Add absolue path. (ex. c:/lib/inc)
EXT_INC =

################  Library directory.
# Add relative path from $(PRJ_TOP)
PRJ_LIB_DIR =

# Add Absolute path
EXT_LIB_DIR = 

LIB_FILE = c

################ source files ##############
# Source file들은 project TOP 에서의 위치를 나타낸다.
CSRC =	\
		startup/startup_stm32f103x6.c \
		hal/stm32f1xx_hal.c \
		hal/stm32f1xx_hal_cortex.c \
		hal/stm32f1xx_hal_rcc.c \
		hal/stm32f1xx_hal_gpio.c \
		hal/stm32f1xx_hal_uart.c \
		app/main.c \
		app/print.c \
		app/sysmem.c \
		app/stm32f1xx_hal_msp.c \
		app/system_stm32f1xx.c \
		app/stm32f1xx_it.c

CPPSRC =

ASRC =
#	startup/startup_stm32f103c8tx.S
