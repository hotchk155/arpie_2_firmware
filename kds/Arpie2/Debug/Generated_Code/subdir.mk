################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Generated_Code/Vectors.cpp 

C_SRCS += \
../Generated_Code/Cpu.c \
../Generated_Code/FLASH1.c \
../Generated_Code/GPIO1.c \
../Generated_Code/I2CBus.c \
../Generated_Code/KBI0.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/TU1.c \
../Generated_Code/UART0.c 

OBJS += \
./Generated_Code/Cpu.o \
./Generated_Code/FLASH1.o \
./Generated_Code/GPIO1.o \
./Generated_Code/I2CBus.o \
./Generated_Code/KBI0.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/TU1.o \
./Generated_Code/UART0.o \
./Generated_Code/Vectors.o 

C_DEPS += \
./Generated_Code/Cpu.d \
./Generated_Code/FLASH1.d \
./Generated_Code/GPIO1.d \
./Generated_Code/I2CBus.d \
./Generated_Code/KBI0.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/TU1.d \
./Generated_Code/UART0.d 

CPP_DEPS += \
./Generated_Code/Vectors.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Static_Code/PDD" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Static_Code/IO_Map" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Sources" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/%.o: ../Generated_Code/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Static_Code/PDD" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Static_Code/IO_Map" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Sources" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Generated_Code" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


