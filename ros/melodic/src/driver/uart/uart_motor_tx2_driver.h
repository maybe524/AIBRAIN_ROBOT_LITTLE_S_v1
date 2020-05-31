//
// Created by 1 on 2020/5/6.
//

#ifndef EXAMPLE_UART_MOTOR_TX2_DRIVER_H
#define EXAMPLE_UART_MOTOR_TX2_DRIVER_H

#include "string.h"
#include "resource_core.h"

#define UART_MOTOR_TX2_TAG  "uartmotor"

#define UART_MOTOR_CTL_SET_EVENT_CB             0x01
/*
*  开始启动处理指令的程序
*/
#define UART_MOTOR_CTL_START_NOTIFY             0x02


// UART的回调消息分类
#define UART_MOTOR_NOTIFY_ID_RECV               0x01
// 获取uartMotor的初始化状态等
#define UART_MOTOR_NOTIFY_ID_STATUS             0x02


#define UART_MOTOR_STATUS_INITOK                0x01    //  表示初始化OK

typedef int (*uart_motor_async_cb)(int eventId, void *data, unsigned int len);

struct uart_motor_prcdata {
    int portFd;
    int portId;
    bool isNotifyBusy;
    bool isNeedCancelNotify;

    uart_motor_async_cb cb;
};

class CUartMotorTX2Resource: public CResource {
public:
    CUartMotorTX2Resource() {};
    CUartMotorTX2Resource(char *n, int port);
   ~CUartMotorTX2Resource() {};

    int open (char *userName, unsigned int flags);
    int read (void *data, unsigned int len, unsigned int flags);
    int ctrl (unsigned int cmd, void *data, unsigned int len, unsigned int flags);
    int write(void *data, unsigned int len, unsigned int flags);
    int close(char *userName, unsigned int flags);

    struct uart_motor_prcdata data;
};

int driver_uart_tx2_init(unsigned int flags);

#endif //EXAMPLE_UART_MOTOR_TX2_DRIVER_H
