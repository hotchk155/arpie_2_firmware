################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Sources/Display.cpp \
../Sources/Events.cpp \
../Sources/Keyboard.cpp \
../Sources/i2c.cpp \
../Sources/main.cpp 

OBJS += \
./Sources/Display.o \
./Sources/Events.o \
./Sources/Keyboard.o \
./Sources/i2c.o \
./Sources/main.o 

CPP_DEPS += \
./Sources/Display.d \
./Sources/Events.d \
./Sources/Keyboard.d \
./Sources/i2c.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Static_Code/PDD" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Static_Code/IO_Map" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Sources" -I"D:/GitHub/arpie_2_firmware/kds/Arpie2/Generated_Code" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


