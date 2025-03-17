################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

SREC += \
bootloader_ek_ra6m3.srec 

MAP += \
bootloader_ek_ra6m3.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/src" -I"." -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/inc/api" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/inc/instances" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_gen" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/fsp_cfg/bsp" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/fsp_cfg" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/mcu-tools/MCUboot/boot/bootutil/src" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/mcu-tools/include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/mcu-tools/MCUboot/boot/bootutil/include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/arm" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/arm/mbedtls/include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/arm/mbedtls/library" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/crypto_procedures/src/sce7/plainkey/private/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/crypto_procedures/src/sce7/plainkey/public/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/crypto_procedures/src/sce7/plainkey/primitive" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/common" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/rm_mcuboot_port" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/rm_psa_crypto/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/mcu-tools/include/mcuboot_config" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/mcu-tools/include/sysflash" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/arm/mbedtls" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

