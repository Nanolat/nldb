################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nldb_main.cpp \
../nldb_server.cpp 

OBJS += \
./nldb_main.o \
./nldb_server.o 

CPP_DEPS += \
./nldb_main.d \
./nldb_server.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ladmin/nldb_work/nldb/src/include" -I"/home/ladmin/nldb_work/nldb/src/txbase" -O3 -Wall -c -fmessage-length=0 -std=c++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


