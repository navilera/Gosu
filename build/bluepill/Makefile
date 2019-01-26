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
ASM_OBJS = $(patsubst $(PRJ_HOME)/boot/$(TARGET)/%.S, $(OUTPUT_DIR)/%.os, $(ASM_SRCS))

VPATH = boot/$(TARGET) 			\
        hal/$(TARGET)	                \
        hal/src	                        \
        lib				\
        lib/$(ARM_ARCH)			\
        kernel

C_SRCS  = $(notdir $(wildcard $(PRJ_HOME)/boot/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard $(PRJ_HOME)/hal/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard $(PRJ_HOME)/hal/src/*.c))
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

COMMON_FLG = -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb-interwork -mlittle-endian
COMMON_FLG += -mcpu=$(MCPU) -m$(ARMTHMODE)

CFLAGS = -c -g -std=c11 
CFLAGS += -O2 -Wall
CFLAGS += $(USR_DEFINE) $(INC_DIRS)
CFLAGS += $(COMMON_FLG) 

LDFLAGS = -Wl,--gc-sections $(COMMON_FLG)

navilos = $(OUTPUT_DIR)/$(OUTNAME).axf
navilos_bin = $(OUTPUT_DIR)/$(OUTNAME).bin

.PHONY: all clean run debug gdb

all: $(navilos)

burn: $(navilos)
	sudo stm32flash -w $(navilos_bin) -v -g 0x0 /dev/ttyUSB0
    

clean:
	@echo "Delete.. " $(OUTPUT_DIR) 
	@rm -fr $(OUTPUT_DIR) 
	
$(navilos): $(ASM_OBJS) $(C_OBJS)
	@echo "Linking.."
	@echo "Generate binary.. " $(navilos_bin)
	@$(LD) $(ASM_OBJS) $(C_OBJS) -o $(navilos) -n -T $(LINKER_SCRIPT) -Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	@$(OD) -t $(navilos) > $(SYM_FILE)
	@$(OC) -O binary $(navilos) $(navilos_bin)
	
$(OUTPUT_DIR)/%.os: %.S
	@echo "compile " $<
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -o $@ $<
	
$(OUTPUT_DIR)/%.o: %.c
	@echo "compile " $<
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -o $@ $<
	
