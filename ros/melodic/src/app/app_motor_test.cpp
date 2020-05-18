//
// Created by 1 on 2020/5/18.
//

#include "common.h"
#include "app_motor_test.h"

static CResourceUser motorResourceUser;

int app_motor_test(unsigned int flags)
{
    int ret;
    CResourceUser *motorUserPtr = &motorResourceUser;

    logInfo(APP_MOTOR_TAG, "app_motor_test");
    ret = resoucreUserRegister(motorUserPtr);
    logInfo(APP_MOTOR_TAG, "resoucreUserRegister ret: %d", ret);

    ret = resoucreOpen(motorUserPtr, "res/uart/motor");
    logInfo(APP_MOTOR_TAG, "resoucreOpen ret: %d", ret);

    return 0;
}