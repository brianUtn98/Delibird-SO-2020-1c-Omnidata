################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/commons/collections/dictionary.c \
../src/commons/collections/list.c \
../src/commons/collections/queue.c 

OBJS += \
./src/commons/collections/dictionary.o \
./src/commons/collections/list.o \
./src/commons/collections/queue.o 

C_DEPS += \
./src/commons/collections/dictionary.d \
./src/commons/collections/list.d \
./src/commons/collections/queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/commons/collections/%.o: ../src/commons/collections/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


