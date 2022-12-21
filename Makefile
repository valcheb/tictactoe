#
# Makefile for stm32f746 for TicTacToe
#

.PHONY: all clean

#Instruments
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

#Linker script for mcu
LDSCRIPT := Drivers/CMSIS/Device/STM32F746NGHx_FLASH.ld

#Project name
TARGET := ttt

#Build paths
BUILD := build

#Requared paths
SOURCED := . app
CMSISD := Drivers/CMSIS/Core Drivers/CMSIS/Device
SOURCED += $(CMSISD)

#Prepare source files from directories
SOURCEF := $(wildcard $(addsuffix /*.c, $(SOURCED)))
OBJF := $(notdir $(SOURCEF))
OBJF := $(OBJF:.c=.o)
OBJF := $(addprefix $(BUILD)/,$(OBJF))

SUF := $(wildcard $(addsuffix /*.s, $(SOURCED)))
SUOBJF := $(notdir $(SUF))
SUOBJF := $(SUOBJF:.s=.o)
SUOBJF := $(addprefix $(BUILD)/,$(SUOBJF))

#Compile Flags
CFLAGS := -g -std=c99 -O0 -fdata-sections -ffunction-sections
#CFLAGS += -flto
CFLAGS += -Wall -Wno-missing-braces -Wextra -Warray-bounds -Wno-unused-parameter
CFLAGS += $(addprefix -I, $(SOURCED))
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=soft
#hardware float point? -mfpu=fpv5-sp-d16 -mfloat-abi=hard
CFLAGS += -DSTM32F746xx -DUSE_HAL_DRIVER

#Link Flags
LDFLAGS := -Wl,--gc-sections,-Map,$(BUILD)/$(TARGET).map -g -T$(LDSCRIPT) --specs=nosys.specs --specs=nano.specs

#Headers of libraries
HALLLD := Drivers/STM32F7xx_HAL_Driver
BSPD := Drivers/BSP/Components/rk043fn48h Drivers/BSP/STM32746G-Discovery
CFLAGS += -I$(HALLLD)/Inc $(addprefix -I, $(BSPD))

#List of used library object files
OBJF += $(BUILD)/stm32f7xx_hal_gpio.o $(BUILD)/stm32f7xx_hal_rcc.o $(BUILD)/stm32f7xx_hal.o $(BUILD)/stm32f7xx_hal_cortex.o
OBJF += $(BUILD)/stm32f7xx_hal_pwr_ex.o $(BUILD)/stm32f7xx_hal_dma.o $(BUILD)/stm32f7xx_hal_dma2d.o $(BUILD)/stm32f7xx_hal_ltdc.o
OBJF += $(BUILD)/stm32f7xx_hal_sdram.o $(BUILD)/stm32746g_discovery_sdram.o $(BUILD)/stm32746g_discovery_lcd.o
OBJF += $(BUILD)/stm32f7xx_ll_fmc.o $(BUILD)/stm32f7xx_hal_rcc_ex.o
#Paths for compilator
VPATH := $(SOURCED)
#path for lib source
VPATH += $(HALLLD)/Src $(BSPD)

#Rules
all: mkdir $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).hex

mkdir:
	mkdir -p ./$(BUILD)

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

$(BUILD)/%.elf: suobj obj
	$(CC) $(CFLAGS) -o $@ $(SUOBJF) $(OBJF) $(LDFLAGS)

suobj: $(SUOBJF)

$(BUILD)/%.o: %.s
	$(CC) -c -MD $(CFLAGS) -o $@ $^

obj: $(OBJF)

$(BUILD)/%.o: %.c
	$(CC) -c -MD $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD)

include $(wildcard *.d)
