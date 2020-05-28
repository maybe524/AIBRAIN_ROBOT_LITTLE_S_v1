//
// Created by 1 on 2020/5/18.
//

#include "common.h"
#include "app_motor_test.h"

static CResourceUser motorResourceUser;
static char *resName = "res/uart/motor0";
static char *atLog = "AT+LOG=1\r\n";

int app_motor_test(unsigned int flags)
{
    int ret;
    char buff[1024] = {0};
    CResourceUser *resUser = &motorResourceUser;

    logInfo(APP_MOTOR_TAG, "app_motor_test");
    ret = resourceUserRegister(resUser);
    logInfo(APP_MOTOR_TAG, "resourceUserRegister ret: %d", ret);

    ret = resourceOpen(resUser, resName);
    logInfo(APP_MOTOR_TAG, "resourceOpen ret: %d", ret);

    while (true) {
        ret = resourceWrite(resUser, resName, atLog, strlen(atLog), 0);
        logInfo(APP_MOTOR_TAG, "ret: %d, write", ret);
        memset(buff, 0, sizeof(buff));
        ret = resourceRead(resUser, resName, buff, sizeof(buff), 0);
        logInfo(APP_MOTOR_TAG, "ret: %d, buff: %s", ret, buff);
    }

    return 0;
}