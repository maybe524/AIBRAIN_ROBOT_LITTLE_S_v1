//
// Created by 1 on 2020/5/6.
//

#ifndef EXAMPLE_UART_MOTOR_TX2_DRIVER_H
#define EXAMPLE_UART_MOTOR_TX2_DRIVER_H

#include "resource_core.h"

class CUartMotorTX2Resource: public CResource {
public:
    CUartMotorTX2Resource();
   ~CUartMotorTX2Resource();

    int open (char *userName, unsigned int flags);
    int read (void *data, unsigned int len, unsigned int flags);
    int ctrl (unsigned int cmd, void *data, unsigned int len, unsigned int flags);
    int write(void *data, unsigned int len, unsigned int flags);
    int close(char *userName, unsigned int flags);
};

#define UART_MOTOR_TX2_TAG  "uartmotor"

#endif //EXAMPLE_UART_MOTOR_TX2_DRIVER_H
