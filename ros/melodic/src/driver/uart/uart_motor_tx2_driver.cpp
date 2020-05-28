#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <termios.h> //set baud rate

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <resource_core.h>

#include "log.h"
#include "common_core.h"
#include "uart_motor_tx2_driver.h"

//#define rec_buf_wait_2s 2
#define buffLen     1024
#define rcvTimeOut  2

static CUartMotorTX2Resource uartMotorPort0("res/uart/motor0", 2);      // 把Uart的dev/ttyTHS2映射到资源的“res/uart/motor0”。
static CUartMotorTX2Resource uartMotorPort1("res/uart/motor1", 1);      // 把Uart的dev/ttyTHS1映射到资源的“res/uart/motor1”。

int read_data_tty(int fd, char *rec_buf, int rec_wait);
int device_485_receive(int fd);
static int uartGetPortFd(int comport);
int uartSetOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
int uartGetDataStream(int fd, char *rcv_buf, int time_out, int llen);
int uartSetDataStream(int fd, char *send_buf, int len);

CUartMotorTX2Resource::CUartMotorTX2Resource(char *n, int port)
{
    this->name = n;
    memset(&data, 0, sizeof(struct uart_motor_prcdata));
    this->data.portId = port;
};

int CUartMotorTX2Resource::open(char *resName, unsigned int flags)
{
    int ret, fd = -1;

    logInfo(UART_MOTOR_TX2_TAG, "uart open, port: %d", this->data.portId);
    fd = uartGetPortFd(this->data.portId);
    if (fd < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "open_port error");
        return -1;
    }
    ret = uartSetOpt(fd, 115200, 8, 'N', 1);
    if (ret < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "set_opt error");
        return -1;
    }
    tcflush(fd, TCIOFLUSH);
    fcntl(fd, F_SETFL, 0);
    this->data.portFd = fd;
    logInfo(UART_MOTOR_TX2_TAG, "serial fdSerial = %d, done", this->data.portFd);

    return 0;
}

int CUartMotorTX2Resource::read(void *data, unsigned int len, unsigned int flags)
{
    int ret;

    if (this->data.portFd <= 0)
        return -EINVAL;
    ret = uartGetDataStream(this->data.portFd, (char *)data, 10, len);

    return 0;
}

int CUartMotorTX2Resource::ctrl(unsigned int cmd, void *data, unsigned int len, unsigned int flags)
{
    int ret = -EINVAL;

    switch (cmd) {
    case UART_MOTOR_CTL_SET_ASYNC_CB:
        if (!data || this->data.portFd <= 0)
            break;
        this->data.cb = (uart_motor_async_cb *)data;
        break;
    }
    return ret;
}

int CUartMotorTX2Resource::write(void *data, unsigned int len, unsigned int flags)
{
    int ret;

    logInfo(UART_MOTOR_TX2_TAG, "write, portFd: %d", this->data.portFd);
    if (this->data.portFd <= 0)
        return -EINVAL;
    ret = uartSetDataStream(this->data.portFd, (char *)data, len);
    return 0;
}

int CUartMotorTX2Resource::close(char *userName, unsigned int flags)
{
    logInfo(UART_MOTOR_TX2_TAG, "close, portFd: %d", this->data.portFd);
    if (this->data.portFd <= 0)
        return -EINVAL;

    // close(this->data.portFd);
    this->data.portFd = 0;
    return 0;
}

int read_data_tty(int fd, char *rec_buf, int rec_wait)
{
    int retval;
    fd_set rfds;
    struct timeval tv;
    int ret, pos;

    tv.tv_sec = rec_wait;
    tv.tv_usec = 0;
    pos = 0;

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        retval = select(fd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) {
            logInfo(UART_MOTOR_TX2_TAG, "select()");
            break;
        }
		else if (retval) {
            ret = read(fd, rec_buf + pos, 2048);
            pos += ret;
            if (rec_buf[pos - 2] == '\r' && rec_buf[pos - 1] == '\n') {
                FD_ZERO(&rfds);
                FD_SET(fd, &rfds);
                retval = select(fd + 1, &rfds, NULL, NULL, &tv);

                if (!retval) {
                    break;
                }
            }
        }
		else {
            break;
        }
    }

    return 1;
}

int device_485_receive(int fd)
{
    int ret;
    char rec_buf[1024];
    int i;
    char send_buf[] = {"02030202f925"};

    for (i = 0; i < 10; i++) {
        /*ret = write(fd, send_buf, strlen(send_buf));
        if (ret == -1) {
            logInfo(UART_MOTOR_TX2_TAG, "write device %s error", device);
            return -1;
        }*/

        //if (read_data_tty(fd, rec_buf, rec_buf_wait_2s)) {
        if (read_data_tty(fd, rec_buf, 2)) {
            logInfo(UART_MOTOR_TX2_TAG, "%s", rec_buf);
        } else {
            logInfo(UART_MOTOR_TX2_TAG, "read_error");
        }

        //if ((read(fd, rec_buf, strlen(rec_buf))) == -1) {
        //      logInfo(UART_MOTOR_TX2_TAG, "error reading string");
        //      return -1;
        //} else {
        //      logInfo(UART_MOTOR_TX2_TAG, "%s", rec_buf);
    }
    return 0;
}

static int uartGetPortFd(int comport)
{
    int fd = 0, ret;
    char buff[32] = {0};

    sprintf(buff, "/dev/ttyTHS%d", comport);
    fd = open(buff, O_RDWR | O_NOCTTY | O_NDELAY);
    logInfo(UART_MOTOR_TX2_TAG, "open serial port: %s, ret: %d", buff, fd);
    if (fd <= 0)
        return -1;
    ret = fcntl(fd, F_SETFL, 0);
    logInfo(UART_MOTOR_TX2_TAG, "fcntl ret: %d!", ret);
    ret = isatty(STDIN_FILENO);
    logInfo(UART_MOTOR_TX2_TAG, "%s", !ret ? "standard input is not a terminal device" : "is a tty success");
    logInfo(UART_MOTOR_TX2_TAG, "fd-open: %d", fd);

    return fd;
}

int uartSetOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    int ret;
    struct termios newtio, oldtio;

    ret = tcgetattr(fd, &oldtio);
    if (ret != 0) {
        logInfo(UART_MOTOR_TX2_TAG, "setupSerial 1");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (nBits) {
    case 7:
        newtio.c_cflag |= CS7;
        break;
	case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch (nEvent) {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
       	newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch (nSpeed) {
    case 2400:
        cfsetispeed(&newtio, B2400); cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800); cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600); cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200); cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600); cfsetospeed(&newtio, B9600);
        break;
    }
    if (nStop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if (nStop == 2)
        newtio.c_cflag |= CSTOPB;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
        logInfo(UART_MOTOR_TX2_TAG, "com set error");
        return -1;
    }
    logInfo(UART_MOTOR_TX2_TAG, "set done!");

    return 0;
}

int uartGetDataStream(int fd, char *buff, int time_out, int len)
{
    int retval, ret, pos = 0;
    fd_set rfds;
    struct timeval tv;
    tv.tv_sec = time_out / 1000;  // set the rcv wait time
    tv.tv_usec = time_out % 1000 * 1000;  // 100000us = 0.1s

    while (true) {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        retval = select(fd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1) {
            logInfo(UART_MOTOR_TX2_TAG, "select = -1");
            break;
        }
		else if (retval) {
            ret = read(fd, buff + pos, 1);
            if (-1 == ret)
                break;
            pos++;
            if (len <= pos)
                break;
        }
		else {
            break;
        }
    }

    return pos;
}

int uartSetDataStream(int fd, char *buff, int len)
{
    ssize_t ret;

    if (!buff || !fd || !len)
        return -EINVAL;
    ret = write(fd, buff, len);
    if (ret == -1) {
        logInfo(UART_MOTOR_TX2_TAG, "write device error");
        return ret;
    }

    return ret;
}

int driver_uart_tx2_init(unsigned int flags)
{
    int ret;

    logInfo(UART_MOTOR_TX2_TAG, "driver_uart_tx2 start");
    ret = resourceRegister(&uartMotorPort0);
    logInfo(UART_MOTOR_TX2_TAG, "uart port0 ret: %d", ret);
    ret = resourceRegister(&uartMotorPort1);
    logInfo(UART_MOTOR_TX2_TAG, "uart port1 ret: %d", ret);

    return 0;
}

AIBRAIN_CORE_MODULE_INIT(driver_uart_tx2_init);