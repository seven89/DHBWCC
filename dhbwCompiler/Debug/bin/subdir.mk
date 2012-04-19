################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../bin/diag.o \
../bin/ir_code_generation.o \
../bin/main.o \
../bin/parser.o \
../bin/resource_manager.o \
../bin/scanner.o \
../bin/symboltable.o 

C_SRCS += \
../bin/parser.c \
../bin/scanner.c 

OBJS += \
./bin/parser.o \
./bin/scanner.o 

C_DEPS += \
./bin/parser.d \
./bin/scanner.d 


# Each subdirectory must supply rules for building sources it contributes
bin/%.o: ../bin/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


