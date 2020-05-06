//
// Created by 1 on 2020/5/6.
//

#include "log.h"

#defien DRIVER_MODULE_TAG   "driverMdl"

int driver_module_init(unsigned int flags)
{
    int ret;

    logInfo(DRIVER_MODULE_TAG, "driver module init, start");
    driver_uart_tx2_init(0);

    return ret;
}