//
// Created by 1 on 2020/5/18.
//

#include "common.h"
#include "app_motor_test.h"
#include "uart_motor_tx2_driver.h"

static CResourceUser motorResourceUser;
static char *resName = "res/uart/motor0";
static char *atLog = "AT+LOG=1\r\n";
static bool isUartDriverOK = false;

static int uart_motor_user_async_cb(int eventId, void *data, unsigned int len)
{
    char buff[1024] = {0};
    int i;

    logInfo(APP_MOTOR_TAG, "eventId: %d, data: %p, len: %d", eventId, data, len);
    switch (eventId) {
        case UART_MOTOR_NOTIFY_ID_STATUS: {
            unsigned int initStatus = *(unsigned int *)data;
            isUartDriverOK = initStatus == UART_MOTOR_STATUS_INITOK ? true : false;
            break;
        }
        case UART_MOTOR_NOTIFY_ID_RECV: {
            memset(buff, 0, sizeof(buff));
            strncpy(buff, (char *)data, len);
            logInfo(APP_MOTOR_TAG, "UART_MOTOR_NOTIFY_ID_RECV: %s", buff);
            break;
        }
        default:
            break;
    }
}

int app_motor_test(unsigned int flags)
{
    int ret, timeout = 10;
    char buff[1024] = {0};
    CResourceUser *resUser = &motorResourceUser;

    logInfo(APP_MOTOR_TAG, "app_motor_test");
    ret = resourceUserRegister(resUser);
    logInfo(APP_MOTOR_TAG, "resourceUserRegister ret: %d", ret);

    ret = resourceOpen(resUser, resName);
    logInfo(APP_MOTOR_TAG, "resourceOpen ret: %d", ret);
    ret = resourceCtrl(resUser, resName, UART_MOTOR_CTL_SET_EVENT_CB, (void *)uart_motor_user_async_cb, 0, 0);
    ret = resourceCtrl(resUser, resName, UART_MOTOR_CTL_START_NOTIFY, 0, 0, 0);
    while (!isUartDriverOK && timeout--)
        sleep(1);
    logInfo(APP_MOTOR_TAG, "ready: %d, timeout: %d", isUartDriverOK, timeout);

    while (true) {
        ret = resourceWrite(resUser, resName, atLog, strlen(atLog), 0);
        logInfo(APP_MOTOR_TAG, "ret: %d, write", ret);
        sleep(5);
#if 0
        memset(buff, 0, sizeof(buff));
        ret = resourceRead(resUser, resName, buff, sizeof(buff), 0);
        logInfo(APP_MOTOR_TAG, "ret: %d, buff: %s", ret, buff);
#endif
    }

    return 0;
}