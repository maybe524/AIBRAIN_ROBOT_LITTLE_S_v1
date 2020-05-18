//
// Created by 1 on 2020/5/6.
//

#include "log.h"
#include "uart_motor_tx2_driver.h"

#define DRIVER_MODULE_TAG   "driverMdl"

int driverModuleInit(unsigned int flags)
{
    int ret;

    logInfo(DRIVER_MODULE_TAG, "driver module init, start");
    driver_uart_tx2_init(0);

    return ret;
}