################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nldb.cpp \
../nldb_plugin_array_tree.cpp \
../nldb_plugin_leveldb.cpp \
../nldb_plugin_tc.cpp \
../nldb_repl_slave_thread.cpp 

OBJS += \
./nldb.o \
./nldb_plugin_array_tree.o \
./nldb_plugin_leveldb.o \
./nldb_plugin_tc.o \
./nldb_repl_slave_thread.o 

CPP_DEPS += \
./nldb.d \
./nldb_plugin_array_tree.d \
./nldb_plugin_leveldb.d \
./nldb_plugin_tc.d \
./nldb_repl_slave_thread.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_GLIBCXX_USE_NANOSLEEP -I"../../include" -I"../../txbase" -I"../../disruptor--" -I"../../../external-libs/include" -O3 -Wall -c -fmessage-length=0 -std=c++0x  -static-libgcc -static-libstdc++ -nodefaultlibs -shared -fPIC -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


