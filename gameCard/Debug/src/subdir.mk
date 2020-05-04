################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/funcionesGameCard.c \
../src/gameCard.c 

OBJS += \
./src/funcionesGameCard.o \
./src/gameCard.o 

C_DEPS += \
./src/funcionesGameCard.d \
./src/gameCard.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2020-1c-Omnidata/commons" -I"/home/utnso/workspace/tp-2020-1c-Omnidata/MiLibreria" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


