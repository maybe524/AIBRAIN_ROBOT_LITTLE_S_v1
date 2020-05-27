//
// Created by 1 on 2020/5/6.
//

#ifndef EXAMPLE_UART_MOTOR_TX2_DRIVER_H
#define EXAMPLE_UART_MOTOR_TX2_DRIVER_H

#include "string.h"
#include "resource_core.h"

#define UART_MOTOR_TX2_TAG  "uartmotor"

struct uart_motor_prcdata {
    int portFd;
};

class CUartMotorTX2Resource: public CResource {
public:
    CUartMotorTX2Resource() {};
    CUartMotorTX2Resource(char *n) { this->name = n; memset(&data, 0, sizeof(struct uart_motor_prcdata)); };
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
