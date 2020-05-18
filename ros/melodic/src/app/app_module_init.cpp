//
// Created by 1 on 2020/5/18.
//

#include "common.h"
#include "app_module_init.h"

int appModuleInit(unsigned int flags)
{
    logInfo(APP_MODULE_TAG, "app_module_init");

    app_motor_test(0);

    return 0;
}