################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../array_tree_test.cpp \
../main.cpp \
../nldb_cursor_dirty_test.cpp \
../nldb_cursor_test.cpp \
../nldb_data.cpp \
../nldb_order_test.cpp \
../nldb_test.cpp \
../plugin_array_tree_test.cpp \
../sorted_array_test.cpp \
../tree_internal_node_test.cpp \
../tree_leaf_node_test.cpp \
../tree_node_test.cpp \
../tree_test_common.cpp 

OBJS += \
./array_tree_test.o \
./main.o \
./nldb_cursor_dirty_test.o \
./nldb_cursor_test.o \
./nldb_data.o \
./nldb_order_test.o \
./nldb_test.o \
./plugin_array_tree_test.o \
./sorted_array_test.o \
./tree_internal_node_test.o \
./tree_leaf_node_test.o \
./tree_node_test.o \
./tree_test_common.o 

CPP_DEPS += \
./array_tree_test.d \
./main.d \
./nldb_cursor_dirty_test.d \
./nldb_cursor_test.d \
./nldb_data.d \
./nldb_order_test.d \
./nldb_test.d \
./plugin_array_tree_test.d \
./sorted_array_test.d \
./tree_internal_node_test.d \
./tree_leaf_node_test.d \
./tree_node_test.d \
./tree_test_common.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_GLIBCXX_USE_NANOSLEEP -I"../../../external-libs/include" -I"../../include" -I"../../txbase" -I"../../nldb" -O3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


