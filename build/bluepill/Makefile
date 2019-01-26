MCPU = cortex-m4
ARMTHMODE = thumb
ARM_ARCH = cortexM

SOC = stm32f10c8
TARGET = bluepill
OUTNAME = gosu

USR_DEFINE = -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump

PRJ_HOME = .
OUTPUT_DIR = $(PRJ_HOME)/out

LINKER_SCRIPT = $(PRJ_HOME)/build/$(TARGET)/$(SOC).ld
MAP_FILE = $(OUTPUT_DIR)/$(OUTNAME).map
SYM_FILE = $(OUTPUT_DIR)/$(OUTNAME).sym

ASM_SRCS = $(wildcard $(PRJ_HOME)/boot/$(TARGET)/*.S)
ASM_OBJS = $(patsubst %.S, $(OUTPUT_DIR)/%.os, $(ASM_SRCS))

VPATH = boot 			\
        hal/$(TARGET)	\
        lib				\
        lib/$(ARM_ARCH)				\
        kernel

C_SRCS  = $(notdir $(wildcard $(PRJ_HOME)/boot/*.c))
C_SRCS += $(notdir $(wildcard $(PRJ_HOME)/hal/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard $(PRJ_HOME)/lib/*.c))
C_SRCS += $(notdir $(wildcard $(PRJ_HOME)/lib/$(ARM_ARCH)/*.c))
C_SRCS += $(notdir $(wildcard $(PRJ_HOME)/kernel/*.c))
C_OBJS = $(patsubst %.c, $(OUTPUT_DIR)/%.o, $(C_SRCS))

INC_DIRS  = -I boot/$(TARGET)           \
            -I include 			\
            -I hal	   		\
            -I hal/$(TARGET)	        \
            -I lib			\
            -I lib/$(ARM_ARCH)		\
            -I kernel

CFLAGS = -c -g -std=c11 -mthumb-interwork -mlittle-endian
CFLAGS += -O2 -Wall
CFLAGS += -mcpu=$(MCPU) -m$(ARMTHMODE) $(INC_DIRS)
CFLAGS += -Wl,--gc-sections
CFLAGS += $(USR_DEFINE)

LDFLAGS = -nostartfiles -nostdlib -nodefaultlibs -static -lgcc

navilos = $(OUTPUT_DIR)/$(OUTNAME).axf
navilos_bin = $(OUTPUT_DIR)/$(OUTNAME).bin

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -fr out 
	
$(navilos): $(ASM_OBJS) $(C_OBJS)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS) $(C_OBJS) -Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	$(OD) -t $(navilos) > $(SYM_FILE)
	$(OC) -O binary $(navilos) $(navilos_bin)
	
$(OUTPUT_DIR)/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $<
	
$(OUTPUT_DIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $<
	
