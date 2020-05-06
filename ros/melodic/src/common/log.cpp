//
// Created by 1 on 2020/5/6.
//
#include <time.h>

static char ____time_str_buff[20];

char *getCurrTime(void)
{
    time_t t;
    struct tm* ltime;

    time(&t);
    ltime = localtime(&t);
    strftime(____time_str_buff, sizeof(____time_str_buff), "%Y-%m-%d %H:%M:%S", ltime);

    return ____time_str_buff;
}
