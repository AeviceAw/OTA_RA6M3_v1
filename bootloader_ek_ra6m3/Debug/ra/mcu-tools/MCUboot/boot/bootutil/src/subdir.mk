################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/mcu-tools/MCUboot/boot/bootutil/src/boot_record.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/bootutil_misc.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/bootutil_public.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/caps.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/encrypted.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/fault_injection_hardening.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/image_ec.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/image_ec256.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/image_ed25519.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/image_rsa.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/image_validate.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/loader.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/swap_misc.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/swap_move.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/swap_scratch.c \
../ra/mcu-tools/MCUboot/boot/bootutil/src/tlv.c 

C_DEPS += \
./ra/mcu-tools/MCUboot/boot/bootutil/src/boot_record.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/bootutil_misc.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/bootutil_public.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/caps.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/encrypted.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/fault_injection_hardening.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_ec.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_ec256.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_ed25519.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_rsa.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_validate.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/loader.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/swap_misc.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/swap_move.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/swap_scratch.d \
./ra/mcu-tools/MCUboot/boot/bootutil/src/tlv.d 

OBJS += \
./ra/mcu-tools/MCUboot/boot/bootutil/src/boot_record.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/bootutil_misc.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/bootutil_public.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/caps.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/encrypted.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/fault_injection_hardening.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/fault_injection_hardening_delay_rng_mbedtls.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_ec.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_ec256.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_ed25519.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_rsa.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/image_validate.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/loader.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/swap_misc.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/swap_move.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/swap_scratch.o \
./ra/mcu-tools/MCUboot/boot/bootutil/src/tlv.o 

SREC += \
bootloader_ek_ra6m3.srec 

MAP += \
bootloader_ek_ra6m3.map 


# Each subdirectory must supply rules for building sources it contributes
ra/mcu-tools/MCUboot/boot/bootutil/src/%.o: ../ra/mcu-tools/MCUboot/boot/bootutil/src/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM4 -D_RA_ORDINAL=1 -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/src" -I"." -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/inc/api" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/inc/instances" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_gen" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/fsp_cfg/bsp" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/fsp_cfg" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/mcu-tools/MCUboot/boot/bootutil/src" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/mcu-tools/include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/mcu-tools/MCUboot/boot/bootutil/include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/arm" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/arm/mbedtls/include" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/arm/mbedtls/library" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/crypto_procedures/src/sce7/plainkey/private/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/crypto_procedures/src/sce7/plainkey/public/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/crypto_procedures/src/sce7/plainkey/primitive" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce/common" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/r_sce" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/rm_mcuboot_port" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra/fsp/src/rm_psa_crypto/inc" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/mcu-tools/include/mcuboot_config" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/mcu-tools/include/sysflash" -I"C:/Users/liren/Documents/aeviceMD_OTA/bootloader_ek_ra6m3/ra_cfg/arm/mbedtls" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -w --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

