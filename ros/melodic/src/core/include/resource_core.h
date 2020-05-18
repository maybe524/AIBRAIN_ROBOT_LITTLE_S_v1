//
// Created by huangxiaowen on 2020/4/23.
//

#ifndef EXAMPLE_RESOURCE_CORE_H
#define EXAMPLE_RESOURCE_CORE_H

#include <mutex>
#include <iostream>
#include <vector>
#include <pthread.h>

#include "log.h"

using namespace std;

#define RESOURCE_USER_CNT   10
#define RESOURCE_TAG        "resourceT"

class CResourceUser;

class CResource {
public:
    CResource() {};
    CResource(char *resName) {};
   ~CResource() {};

    // resource的操作
    virtual int open (char *userName, unsigned int flags) = 0;
    virtual int read (void *data, unsigned int len, unsigned int flags) = 0;
    virtual int ctrl (unsigned int cmd, void *data, unsigned int len, unsigned int flags) = 0;
    virtual int write(void *data, unsigned int len, unsigned int flags) = 0;
    virtual int close(char *userName, unsigned int flags) = 0;

    char *name;
    int usedCount, currIdx;      // 使用其他资源的次数、被使用的次数
    CResourceUser *used[RESOURCE_USER_CNT];
};

class CResourceUser {
public:
    CResourceUser() {};
   ~CResourceUser() {};

    char *name;
    int useCount, currIdx;
    CResource *use[RESOURCE_USER_CNT];
};

int resoucreRegister(CResource *res);
int resoucreOpen(CResourceUser *resUser, char *resName);
int resoucreCtrl(CResourceUser *resUser, char *resName, unsigned int cmd, void *data, unsigned int len, unsigned int flags);
int resoucreClose(CResourceUser *resUser, char *resName);
int resoucreWrite(CResourceUser *resUser, char *resName, void *data, unsigned int len, unsigned int flags);
int resoucreUserRegister(CResourceUser *resUser);
int resoucreCoreInit(unsigned int flags);

#endif //EXAMPLE_RESOURCE_CORE_H
