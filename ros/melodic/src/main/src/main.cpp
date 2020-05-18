#include <iostream>

#include "resource_core.h"
#include "log.h"
#include "driver_module_init.h"
#include "app_module_init.h"
#include "core_module_init.h"

#define MAIN_TAG    "robotMain"

using namespace std;

int main(int argc, char **argv)
{
    int ret = 0;

    logInfo(MAIN_TAG, "hello little S robot! build time: %s %s", __DATE__, __TIME__);
    // 系统核心初始化
    coreModuleInit(0);
    // 驱动初始化
    driverModuleInit(0);
    // 应用初始化
    appModuleInit(0);

    return 0;
}