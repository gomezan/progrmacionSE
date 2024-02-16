################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/pin_mux.c 

C_DEPS += \
./board/pin_mux.d 

OBJS += \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFRDM_KL46Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\source" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\CMSIS" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\drivers" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\startup" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\utilities" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\board\src" -I"C:\Users\david\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmkl46z_tpm_timer_aaaa\board" -O0 -fno-common -g -gdwarf-4 -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

