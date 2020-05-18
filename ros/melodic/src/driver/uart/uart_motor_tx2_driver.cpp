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
#define buffLen 1024
#define rcvTimeOut 2

static CUartMotorTX2Resource uartMotorTX2Resource("res/uart/motor");

static int openPort(int fd, int comport);
int read_data_tty(int fd, char *rec_buf, int rec_wait);
int device_485_receive(int fd);
static int openPort(int fd, int comport);
int setOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
int readDataTty(int fd, char *rcv_buf, int TimeOut, int Len);
int sendDataTty(int fd, char *send_buf, int Len);

int CUartMotorTX2Resource::open(char *resName, unsigned int flags)
{
    int ret;
    int iSetOpt = 0, fdSerial = 0;
    char buffRcvData[buffLen] = {0};
    unsigned int readDataNum = 0;

    logInfo(UART_MOTOR_TX2_TAG, "CUartMotorTX2Resource::open");
    // openPort
    fdSerial = openPort(fdSerial, 4);
    if (fdSerial < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "open_port error");
        return -1;
    }

    iSetOpt = setOpt(fdSerial, 115200, 8, 'N', 1);
    if (iSetOpt < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "set_opt error");
        return -1;
    }
    logInfo(UART_MOTOR_TX2_TAG, "serial fdSerial=%d", fdSerial);

    tcflush(fdSerial, TCIOFLUSH);	// ������ڻ���
    fcntl(fdSerial, F_SETFL, 0);

    buffRcvData[0] = 's';
    buffRcvData[1] = 't';
    buffRcvData[2] = 'a';
    buffRcvData[3] = 'r';
    buffRcvData[4] = 't';
    ret = sendDataTty(fdSerial, buffRcvData, 5);
    logInfo(UART_MOTOR_TX2_TAG, "ret: %d.", ret);
    while (1) {
        memset(buffRcvData, 0, sizeof(buffRcvData));
        readDataNum = readDataTty(fdSerial, buffRcvData, rcvTimeOut, buffLen);
        logInfo(UART_MOTOR_TX2_TAG, "readDataNum = %d, buffRcvData: %s", readDataNum, buffRcvData);
        // sendDataTty(fdSerial, buffRcvData, readDataNum);
    }

    return 0;
}

int CUartMotorTX2Resource::read(void *data, unsigned int len, unsigned int flags)
{
    return 0;
}

int CUartMotorTX2Resource::ctrl(unsigned int cmd, void *data, unsigned int len, unsigned int flags)
{
    return 0;
}

int CUartMotorTX2Resource::write(void *data, unsigned int len, unsigned int flags)
{
    return 0;
}

int CUartMotorTX2Resource::close(char *userName, unsigned int flags)
{
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

static int openPort(int fd, int comport)
{
    if (comport == 1) {
        fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            logInfo(UART_MOTOR_TX2_TAG, "Can't Open Serial Port");
            return (-1);
        } else {
            logInfo(UART_MOTOR_TX2_TAG, "open ttyS0 .....");
        }
    } else if (comport == 2) {
        fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            logInfo(UART_MOTOR_TX2_TAG, "Can't Open Serial Port");
            return (-1);
        } else {
            logInfo(UART_MOTOR_TX2_TAG, "open ttyS1 .....");
        }
    } else if (comport == 3) {
        fd = open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            logInfo(UART_MOTOR_TX2_TAG, "Can't Open Serial Port");
            return (-1);
        } else {
            logInfo(UART_MOTOR_TX2_TAG, "open ttyS2 .....");
        }
    }
    else if (comport == 4) {
        fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
        if (-1 == fd) {
            logInfo(UART_MOTOR_TX2_TAG, "Can't Open Serial Port");
            return (-1);
        } else {
            logInfo(UART_MOTOR_TX2_TAG, "open ttyUSB0 .....");
        }
    }

    if (fcntl(fd, F_SETFL, 0) < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "fcntl failed!");
    } else {
        logInfo(UART_MOTOR_TX2_TAG, "fcntl=%d", fcntl(fd, F_SETFL, 0));
    }
    if (isatty(STDIN_FILENO) == 0) {
        logInfo(UART_MOTOR_TX2_TAG, "standard input is not a terminal device");
    } else {
        logInfo(UART_MOTOR_TX2_TAG, "is a tty success!");
    }
    logInfo(UART_MOTOR_TX2_TAG, "fd-open=%d", fd);
    return fd;
}

int setOpt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio, oldtio;

    if (tcgetattr(fd, &oldtio) != 0) {
        logInfo(UART_MOTOR_TX2_TAG, "SetupSerial 1");
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
    case 'O':                     //��У��
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
       	newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //żУ��
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //��У��
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch (nSpeed) {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if (nStop == 1) {
        newtio.c_cflag &= ~CSTOPB;
    } else if (nStop == 2) {
        newtio.c_cflag |= CSTOPB;
    }
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

int readDataTty(int fd, char *rcv_buf, int TimeOut, int Len)
{
    int retval;
    fd_set rfds;
    struct timeval tv;
    int ret, pos;
    tv.tv_sec = TimeOut / 1000;  //set the rcv wait time
    tv.tv_usec = TimeOut % 1000 * 1000;  //100000us = 0.1s

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
            ret = read(fd, rcv_buf + pos, 1);
            if (-1 == ret) {
                break;
            }
            pos++;
            if (Len <= pos) {
                break;
            }
        } else {
            break;
        }
    }

    return pos;
}

int sendDataTty(int fd, char *send_buf, int Len)
{
    ssize_t ret;

    ret = write(fd, send_buf, Len);
    if (ret == -1) {
        logInfo(UART_MOTOR_TX2_TAG, "write device error");
        return -1;
    }

    return 1;
}

#if 0
int main(int argc, char** argv)
{
	int ret;
    int iSetOpt = 0, fdSerial = 0;
    char buffRcvData[buffLen] = {0};
    unsigned int readDataNum = 0;

    //openPort
    fdSerial = openPort(fdSerial, 4);
    if (fdSerial < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "open_port error");
        return -1;
    }

	iSetOpt = setOpt(fdSerial, 115200, 8, 'N', 1);
    if (iSetOpt < 0) {
        logInfo(UART_MOTOR_TX2_TAG, "set_opt error");
        return -1;
    }
    logInfo(UART_MOTOR_TX2_TAG, "Serial fdSerial=%d", fdSerial);

    tcflush(fdSerial, TCIOFLUSH);	// ������ڻ���
    fcntl(fdSerial, F_SETFL, 0);

    buffRcvData[0] = 's';
    buffRcvData[1] = 't';
    buffRcvData[2] = 'a';
    buffRcvData[3] = 'r';
    buffRcvData[4] = 't';
    ret = sendDataTty(fdSerial, buffRcvData, 5);
	logInfo(UART_MOTOR_TX2_TAG, "ret: %d.", ret);
    while (1) {
		memset(buffRcvData, 0, sizeof(buffRcvData));
        readDataNum = readDataTty(fdSerial, buffRcvData, rcvTimeOut, buffLen);
		logInfo(UART_MOTOR_TX2_TAG, "readDataNum = %d, buffRcvData: %s", readDataNum, buffRcvData);
        // sendDataTty(fdSerial, buffRcvData, readDataNum);
    }

    return 1;
}
#endif

int driver_uart_tx2_init(unsigned int flags)
{
    int ret;

    logInfo(UART_MOTOR_TX2_TAG, "driver_uart_tx2 start");
    ret = resoucreRegister(&uartMotorTX2Resource);
    logInfo(UART_MOTOR_TX2_TAG, "driver_uart_tx2 ret: %d", ret);

    return 0;
}

AIBRAIN_CORE_MODULE_INIT(driver_uart_tx2_init);