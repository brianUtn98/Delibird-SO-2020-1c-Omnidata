################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/commons/bitarray.c \
../src/commons/config.c \
../src/commons/error.c \
../src/commons/log.c \
../src/commons/memory.c \
../src/commons/process.c \
../src/commons/string.c \
../src/commons/temporal.c \
../src/commons/txt.c 

OBJS += \
./src/commons/bitarray.o \
./src/commons/config.o \
./src/commons/error.o \
./src/commons/log.o \
./src/commons/memory.o \
./src/commons/process.o \
./src/commons/string.o \
./src/commons/temporal.o \
./src/commons/txt.o 

C_DEPS += \
./src/commons/bitarray.d \
./src/commons/config.d \
./src/commons/error.d \
./src/commons/log.d \
./src/commons/memory.d \
./src/commons/process.d \
./src/commons/string.d \
./src/commons/temporal.d \
./src/commons/txt.d 


# Each subdirectory must supply rules for building sources it contributes
src/commons/%.o: ../src/commons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


