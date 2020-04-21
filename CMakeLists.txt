CMAKE_MINIMUM_REQUIRED(VERSION 2.5)
PROJECT(example)

SET(CMAKE_C_COMPILER "gcc")

INCLUDE_DIRECTORIES(
        "./include/"
        "./service/audio/include/"
        "./app/include/"
        "../../../vendor/zib/Audio/include"
        "../../../vendor/zib/external/json-c"
        "../../../external/sqlite/dist"
        "../../../vendor/zib/common"
        "../../../vendor/zib/Resource/include"
        "../../../vendor/longsung/include"
        "../../../vendor/longsung/liblstelephony"
        "../../../vendor/zib/common"
        "./service/wakeup_lock/include"
        "./service/ipc/include"
)
LINK_DIRECTORIES(
        "/usr/arm-linux-gnueabihf/lib"
        "/home/huangxiaowen/Project/dui-sdk/dds-rk3128-sdk-v0.2.6"
)

ADD_COMPILE_OPTIONS(-DUSE_VAD)

ADD_EXECUTABLE(AI
./ros/melodic/src/motor/src/motor_user.cpp
./ros/melodic/src/motor/src/motor_uart_driver.cpp
./ros/melodic/src/motor/src/motor_contrler.cpp
)