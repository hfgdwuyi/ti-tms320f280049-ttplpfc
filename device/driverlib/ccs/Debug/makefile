################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./adc.obj" \
"./asysctl.obj" \
"./can.obj" \
"./cla.obj" \
"./clapromcrc.obj" \
"./clb.obj" \
"./cmpss.obj" \
"./cputimer.obj" \
"./dac.obj" \
"./dcc.obj" \
"./dcsm.obj" \
"./dma.obj" \
"./ecap.obj" \
"./epwm.obj" \
"./eqep.obj" \
"./erad.obj" \
"./flash.obj" \
"./fsi.obj" \
"./gpio.obj" \
"./hrcap.obj" \
"./hrpwm.obj" \
"./i2c.obj" \
"./interrupt.obj" \
"./lin.obj" \
"./memcfg.obj" \
"./pga.obj" \
"./pmbus.obj" \
"./sci.obj" \
"./sdfm.obj" \
"./spi.obj" \
"./sysctl.obj" \
"./version.obj" \
"./xbar.obj" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIB_OUTPUTS += \
driverlib_coff.lib \

LIB_OUTPUTS__QUOTED += \
"driverlib_coff.lib" \


# All Target
all: driverlib_coff.lib

# Tool invocations
driverlib_coff.lib: $(OBJS) $(LIB_SRCS)
ifneq ($(strip $(ORDERED_OBJS)),)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Archiver'
	"C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/ar2000" r "driverlib_coff.lib" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
endif
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(LIB_OUTPUTS__QUOTED)
	-$(RM) "adc.obj" "asysctl.obj" "can.obj" "cla.obj" "clapromcrc.obj" "clb.obj" "cmpss.obj" "cputimer.obj" "dac.obj" "dcc.obj" "dcsm.obj" "dma.obj" "ecap.obj" "epwm.obj" "eqep.obj" "erad.obj" "flash.obj" "fsi.obj" "gpio.obj" "hrcap.obj" "hrpwm.obj" "i2c.obj" "interrupt.obj" "lin.obj" "memcfg.obj" "pga.obj" "pmbus.obj" "sci.obj" "sdfm.obj" "spi.obj" "sysctl.obj" "version.obj" "xbar.obj" 
	-$(RM) "adc.d" "asysctl.d" "can.d" "cla.d" "clapromcrc.d" "clb.d" "cmpss.d" "cputimer.d" "dac.d" "dcc.d" "dcsm.d" "dma.d" "ecap.d" "epwm.d" "eqep.d" "erad.d" "flash.d" "fsi.d" "gpio.d" "hrcap.d" "hrpwm.d" "i2c.d" "interrupt.d" "lin.d" "memcfg.d" "pga.d" "pmbus.d" "sci.d" "sdfm.d" "spi.d" "sysctl.d" "version.d" "xbar.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-if exist driverlib_eabi.lib C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/libinfo2000.exe -o driverlib.lib driverlib_eabi.lib driverlib_coff.lib
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

