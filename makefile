################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := sh
RM := rm -rf

USER_OBJS :=

TARGET   =xplained_pro

#-------------------------------------------------------------------------------
CC       =$(BIN_DIR)arm-none-eabi-gcc 
OBJ_COPY =$(BIN_DIR)arm-none-eabi-objcopy
SIZE     =$(BIN_DIR)arm-none-eabi-size

#-------------------------------------------------------------------------------
include files.mk

COMPILER_OPTS =-x c -mthumb -D__SAML21J18B__ -D__SAML21J18B__ 
COMPILER_OPTS += -DBOARD=SAML21_XPLAINED_PRO -DARM_MATH_CM0PLUS=true 
COMPILER_OPTS += -DUSB_DEVICE_LPM_SUPPORT -DUDD_ENABLE 
COMPILER_OPTS += -DEXTINT_CALLBACK_MODE=true -DTC_ASYNC=true -DSYSTICK_MODE 
COMPILER_OPTS += -DDEBUG  -O1 -fdata-sections -ffunction-sections -mlong-calls 
COMPILER_OPTS += -g3 -Wall -mcpu=cortex-m0plus -c -pipe -fno-strict-aliasing 
COMPILER_OPTS += -Wall -Wstrict-prototypes -Wmissing-prototypes 
COMPILER_OPTS += -Werror-implicit-function-declaration -Wpointer-arith -std=gnu99 
COMPILER_OPTS += -ffunction-sections -fdata-sections -Wchar-subscripts -Wcomment 
COMPILER_OPTS += -Wformat=2 -Wimplicit-int -Wmain -Wparentheses -Wsequence-point 
COMPILER_OPTS += -Wreturn-type -Wswitch -Wtrigraphs -Wunused -Wuninitialized 
COMPILER_OPTS += -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow 
COMPILER_OPTS += -Wbad-function-cast -Wwrite-strings -Wsign-compare 
COMPILER_OPTS += -Wredundant-decls -Wnested-externs -Wlong-long -Wunreachable-code 
COMPILER_OPTS += -Wcast-align --param max-inline-insns-single=500 -MD -MP -MF 
 
##	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="test1.map" --specs=nano.specs -Wl,--start-group -larm_cortexM0l_math -lm  -Wl,--end-group -L"../src/asf/thirdparty/CMSIS/Lib/GCC"  -Wl,--gc-sections -mcpu=cortex-m0plus -Wl,--entry=Reset_Handler -Wl,--cref -mthumb -T../src/asf/sam0/utils/linker_scripts/saml21/gcc/saml21j18b_flash.ld  

LINKER_OPTS =  -mthumb -Wl,-Map="$(TARGET).map" --specs=nano.specs -Wl,--start-group 
LINKER_OPTS += -larm_cortexM0l_math -lm  -Wl,--end-group 
LINKER_OPTS += -L"./src/ASF/thirdparty/CMSIS/Lib/GCC"  -Wl,--gc-sections 
LINKER_OPTS += -mcpu=cortex-m0plus -Wl,--entry=Reset_Handler -Wl,--cref 
LINKER_OPTS += -mthumb -T $(LINKER_FILE)

COMPILER_EXTRA_OPTS=

#-------------------------------------------------------------------------------
OBJS = $(patsubst %.c,%.o,$(wildcard $(C_SRCS)))
OBJS += $(patsubst %.S,%.o,$(wildcard $(ASM_SRCS)))

#-------------------------------------------------------------------------------
all: $(LIBS) $(TARGET).elf secondary-outputs

doc:
	doxygen $(TARGET).doxyfile

$(LIBS):$(LIBS_DEPS) 
	$(MAKE) -C $(dir $@) MCU=$(MCU) CC=$(CC) COMPILER_OPTS='$(COMPILER_OPTS)' -f makefile 

# Tool invocations
$(TARGET).elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM GNU C++ Linker'
	$(CC) $(LINKER_OPTS) -o "$(TARGET).elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(TARGET).hex: $(TARGET).elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	$(OBJ_COPY) -O ihex "$(TARGET).elf"  "$(TARGET).hex"
	@echo 'Finished building: $@'
	@echo ' '

$(TARGET).siz: $(TARGET).elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	$(SIZE) --format=berkeley "$(TARGET).elf"
	@echo 'Finished building: $@'
	@echo 'OBJS: : $(OBJS)'
	@echo 'ASM: : $(ASM_SRCS)'
	@echo ' '

%.o: %.c
	echo 'Building file: $<'
	$(CC) $(INCLUDES) $(COMPILER_OPTS)  $(COMPILER_EXTRA_OPTS) -c "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

# Each subdirectory must supply rules for building sources it contributes
%.o: %.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	$(CC) $(INCLUDES) $(COMPILER_OPTS) $(COMPILER_EXTRA_OPTS) $(COMPILER_ASM_OPTS) -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

secondary-outputs: $(TARGET).hex $(TARGET).siz

clean: 
	find . -name "*.[od]" -exec rm {} \;	
	find . -name "*.elf"  -exec rm {} \;	
	find . -name "*.hex"  -exec rm {} \;	
	find . -name "*.map"  -exec rm {} \;	
	find . -name "*.lss"  -exec rm {} \;	
	rm -fr html
	rm -fr latex
