################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-439974129: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccstheia140/ccs/utils/sysconfig_1.20.0/sysconfig_cli.bat" --script "D:/Ti/TMS320F28004x/siemensV03/c2000.syscfg" -o "syscfg" -s "C:/ti/c2000/C2000Ware_4_03_00_00/.metadata/sdk.json" -d "F28004x" --package F28004x_100PZ --part F28004x_100PZ --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-439974129 ../c2000.syscfg
syscfg/board.h: build-439974129
syscfg/board.cmd.genlibs: build-439974129
syscfg/board.opt: build-439974129
syscfg/pinmux.csv: build-439974129
syscfg/c2000ware_libraries.cmd.genlibs: build-439974129
syscfg/c2000ware_libraries.opt: build-439974129
syscfg/c2000ware_libraries.c: build-439974129
syscfg/c2000ware_libraries.h: build-439974129
syscfg/clocktree.h: build-439974129
syscfg: build-439974129

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccstheia140/ccs/tools/compiler/ti-cgt-c2000_22.6.1A23259/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --idiv_support=idiv0 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --fp_mode=strict --include_path="D:/Ti/TMS320F28004x/pfc_siemensV02/include" --include_path="D:/Ti/TMS320F28004x/siemensV03" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00" --include_path="D:/Ti/TMS320F28004x/siemensV03/device" --include_path="C:/ti/c2000/C2000Ware_4_03_00_00/driverlib/f28004x/driverlib" --include_path="C:/ti/ccstheia140/ccs/tools/compiler/ti-cgt-c2000_22.6.1A23259/include" --define=DEBUG --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="D:/Ti/TMS320F28004x/siemensV03/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


