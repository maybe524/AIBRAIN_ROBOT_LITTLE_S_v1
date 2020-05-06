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
        ros/melodic/src/motor/src/motor_user.cpp
        ros/melodic/src/motor/src/motor_uart_driver.cpp
        ros/melodic/src/motor/src/motor_contrler.cpp
        ros/melodic/src/core/uart_core.cpp ros/melodic/src/core/include/uart_core.h
        ros/melodic/src/core/include/obj_core.h
        ros/melodic/src/core/include/common_core.h ros/melodic/src/driver/uart/uart_motor_tx2_driver.h ros/melodic/src/include/log.h ros/melodic/src/common/log.cpp ros/melodic/src/driver/driver_module_init.cpp ros/melodic/src/driver/include/driver_module_init.h)