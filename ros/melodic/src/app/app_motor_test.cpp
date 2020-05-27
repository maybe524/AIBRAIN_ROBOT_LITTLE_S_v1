//
// Created by 1 on 2020/5/18.
//

#include "common.h"
#include "app_motor_test.h"

static CResourceUser motorResourceUser;
static char *uartPort = "res/uart/motor0";

int app_motor_test(unsigned int flags)
{
    int ret;
    char buff[1024] = {0};
    CResourceUser *resUser = &motorResourceUser;

    logInfo(APP_MOTOR_TAG, "app_motor_test");
    ret = resourceUserRegister(resUser);
    logInfo(APP_MOTOR_TAG, "resourceUserRegister ret: %d", ret);

    ret = resourceOpen(resUser, uartPort);
    logInfo(APP_MOTOR_TAG, "resourceOpen ret: %d", ret);

    while (true) {
        memset(buff, 0, sizeof(buff));
        ret = resourceRead(resUser, uartPort, buff, sizeof(buff), 0);
        logInfo(APP_MOTOR_TAG, "ret: %d, buff: %s", ret, buff);
    }

    return 0;
}