################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/diag.c \
../src/ir_code_generation.c \
../src/lex.yy.c \
../src/main.c \
../src/resource_manager.c \
../src/symboltable.c 

OBJS += \
./src/diag.o \
./src/ir_code_generation.o \
./src/lex.yy.o \
./src/main.o \
./src/resource_manager.o \
./src/symboltable.o 

C_DEPS += \
./src/diag.d \
./src/ir_code_generation.d \
./src/lex.yy.d \
./src/main.d \
./src/resource_manager.d \
./src/symboltable.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


