CMAKE_MINIMUM_REQUIRED(VERSION 2.5)
PROJECT(example)

SET(CMAKE_C_COMPILER "gcc")

INCLUDE_DIRECTORIES(
        "./include/"
        "./src/core/include"
)
LINK_DIRECTORIES(
        "/usr/arm-linux-gnueabihf/lib"
        "/home/huangxiaowen/Project/dui-sdk/dds-rk3128-sdk-v0.2.6"
)

ADD_COMPILE_OPTIONS(-DUSE_VAD)

ADD_EXECUTABLE(AI
        ./ros/melodic/build/CMakeFiles/feature_tests.c
        ./ros/melodic/build/CMakeFiles/3.10.2/CompilerIdC/CMakeCCompilerId.c
        ./ros/melodic/src/core/include/resource_core.h
        ./ros/melodic/src/core/include/obj_core.h
        ./ros/melodic/src/core/include/common_core.h
        ./ros/melodic/src/core/include/uart_core.h
        ./ros/melodic/src/core/include/core_module_init.h
        ./ros/melodic/src/driver/uart/uart_motor_tx2_driver.h
        ./ros/melodic/src/driver/include/driver_module_init.h
        ./ros/melodic/src/app/include/app_module_init.h
        ./ros/melodic/src/app/include/app_motor_test.h
        ./ros/melodic/src/include/common.h
        ./ros/melodic/src/include/log.h
        ./cmake-build-debug/CMakeFiles/3.16.5/CompilerIdC/CMakeCCompilerId.c
        )
