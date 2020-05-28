//
// Created by 1 on 2020/5/6.
//

#ifndef EXAMPLE_UART_MOTOR_TX2_DRIVER_H
#define EXAMPLE_UART_MOTOR_TX2_DRIVER_H

#include "string.h"
#include "resource_core.h"

#define UART_MOTOR_TX2_TAG  "uartmotor"

#define UART_MOTOR_CTL_SET_ASYNC_CB     0x01
/*
*  设置获取每一行数据，每一行数据以\r\n结束。
*  意味着底层要提供该上层以\r\n结束的数据，如果没有\r\n则不返回给应用层。
*/
#define UART_MOTOR_CTL_SET_EVENT_BY_ONELINE     0x02

typedef int (*uart_motor_async_cb)(int eventId, void *data, unsigned int len);

struct uart_motor_prcdata {
    int portFd;
    int portId;
    uart_motor_async_cb *cb;
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

private:
    struct uart_motor_prcdata data;
};

int driver_uart_tx2_init(unsigned int flags);

#endif //EXAMPLE_UART_MOTOR_TX2_DRIVER_H
