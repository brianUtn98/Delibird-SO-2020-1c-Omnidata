################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sockets/servidor.c 

OBJS += \
./sockets/servidor.o 

C_DEPS += \
./sockets/servidor.d 


# Each subdirectory must supply rules for building sources it contributes
sockets/%.o: ../sockets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2020-1c-Omnidata/bibliotecasPropias" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


