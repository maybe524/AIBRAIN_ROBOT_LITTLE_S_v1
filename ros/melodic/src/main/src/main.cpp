#include <iostream>

#include "resource_core.h"
#include "log.h"
#include "driver_module_init.h"

#define MAIN_TAG    "robotMain"

using namespace std;

int main(int argc, char **argv)
{
    int ret = 0;

    logInfo(MAIN_TAG, "hello little S robot! build time: %s", __DATE__);

    // 系统初始化
    resoucreCoreInit(0);


    // 应用初始化

    return 0;
}