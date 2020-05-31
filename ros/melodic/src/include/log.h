//
// Created by 1 on 2020/5/6.
//

#ifndef EXAMPLE_LOG_H
#define EXAMPLE_LOG_H

char *getCurrTime(void);

#include "stdio.h"

#define logInfo(tag, format, argc...)   do { \
        printf("%s [%s] %05d " format "\n", getCurrTime(), (tag), __LINE__, ##argc);} \
    while (0)

#endif //EXAMPLE_LOG_H
