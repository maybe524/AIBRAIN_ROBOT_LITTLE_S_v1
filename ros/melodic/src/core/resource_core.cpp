//
// Created by huangxiaowen on 2020/4/23.
//

#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#include "common_core.h"
#include "resource_core.h"
#include "log.h"

#define RESOURCE_TAG    "resourceT"

static vector<CResource *> sRList;
static vector<CResourceUser *> sUList;
static pthread_mutex_t sRMutex;
static bool isResoucreInited = false;

static int resoucreMutexCheckInit(void)
{
    if (isResoucreInited)
        return 0;
    isResoucreInited = true;
    pthread_mutex_init(&sRMutex, NULL);

    return 0;
}

int resoucreRegister(CResource *res)
{
    int ret = 0;
    vector<CResource *>::iterator iter;

    if (res)
        return -EINVAL;
    resoucreMutexCheckInit();

    pthread_mutex_lock(&sRMutex);
    for (iter = sRList.begin(); iter != sRList.end(); iter++) {
        if (res == (*iter) || !strcmp(res->name, (*iter)->name)) {
            ret = -EBUSY;
            goto register_end;
        }
    }
    memset(res->used, 0, sizeof(CResourceUser *) * RESOURCE_USER_CNT);
    sRList.push_back(res);

register_end:
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreOpen(CResourceUser *resUser, char *resName)
{
    int ret = -EINVAL, idxUse, idxUsed;
    vector<CResource *>::iterator iter;
    CResource *fndRes = NULL;

    if (!resUser || !resName)
        return -EINVAL;
    resoucreMutexCheckInit();

    pthread_mutex_lock(&sRMutex);
    for (iter = sRList.begin(); iter != sRList.end(); iter++) {
        if (!strcmp(resName, (*iter)->name)) {
            fndRes = *iter;
            break;
        }
    }
    if (!fndRes)
        goto open_end;

    for (int i = 0; i < RESOURCE_USER_CNT; i++) {
        if (!resUser->use[i])
            idxUse = i;
        if (!fndRes->used[i])
            idxUsed = i;
        if (resUser->use[i] == fndRes || fndRes->used[i] == resUser) {
            ret = -EBUSY;
            goto open_end;
        }
    }
    if (idxUse < 0 || idxUsed < 0)
        goto open_end;
    // 使用的资源
    ret = fndRes->open(resUser->name, 0);
    if (ret)
        goto open_end;
    resUser->use[idxUse] = fndRes;
    resUser->useCount++;
    fndRes->used[idxUsed] = resUser;    // 被使用的资源设置
    fndRes->usedCount++;     // 记录被使用的次数
    ret = 0;
open_end:
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreCtrl(CResourceUser *resUser, char *resName, unsigned int cmd, void *data, unsigned int len, unsigned int flags)
{
    int ret = -EINVAL;
    vector<CResource *>::iterator iter;

    if (!resUser || !resName)
        return -EINVAL;
    resoucreMutexCheckInit();

    pthread_mutex_lock(&sRMutex);
    for (int i = 0; i < RESOURCE_USER_CNT; i++) {
        if (resUser->use[i] && !strcmp(resUser->use[i]->name, resName)) {
            ret = resUser->use[i]->ctrl(cmd, data, len, flags);
            break;
        }
    }
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreClose(CResourceUser *resUser, char *resName, unsigned int flags)
{
    int ret = -EINVAL, idxUse = -1, idxUsed = -1, i = 0;
    vector<CResource *>::iterator iter;
    CResource *fndRes = NULL;

    if (!resUser || !resName)
        return -EINVAL;
    resoucreMutexCheckInit();

    pthread_mutex_lock(&sRMutex);
    for (iter = sRList.begin(); iter != sRList.end(); iter++) {
        if (!strcmp(resName, (*iter)->name)) {
            fndRes = *iter;
            break;
        }
    }
    if (!fndRes)
        goto close_end;

    for (i = 0; i < RESOURCE_USER_CNT; i++) {
        if (resUser->use[i] == fndRes)
            idxUse = i;
        if (fndRes->used[i] == resUser)
            idxUsed = i;
    }
    if (idxUse < 0 && idxUsed < 0)
        goto close_end;

    ret = fndRes->close(resUser->name, 0);
    if (ret)
        goto close_end;
    resUser->use[idxUse] = NULL;
    fndRes->used[idxUsed] = NULL;
    ret = 0;

close_end:
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreWrite(CResourceUser *resUser, char *resName, void *data, unsigned int len, unsigned int flags)
{
    int ret = -EINVAL;
    vector<CResource *>::iterator iter;

    if (!resUser || !resName)
        return -EINVAL;
    resoucreMutexCheckInit();

    pthread_mutex_lock(&sRMutex);
    for (int i = 0; i < RESOURCE_USER_CNT; i++) {
        if (resUser->use[i] && !strcmp(resUser->use[i]->name, resName)) {
            ret = resUser->use[i]->write(data, len, flags);
            break;
        }
    }
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreDump(void)
{
    int ret = 0;
    vector<CResource *>::iterator iter;

    resoucreMutexCheckInit();
    pthread_mutex_lock(&sRMutex);
    for (iter = sRList.begin(); iter != sRList.end(); iter++) {
        logInfo(RESOURCE_TAG, "resoure name: %s", (*iter)->name);
    }
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreUserRegister(CResourceUser *resUser)
{
    int ret = 0;
    vector<CResourceUser *>::iterator iter;

    if (!resUser)
        return -EINVAL;
    resoucreMutexCheckInit();

    pthread_mutex_lock(&sRMutex);
    for (iter = sUList.begin(); iter != sUList.end(); iter++) {
        if (!strcmp(resUser->name, (*iter)->name)) {
            ret = -EBUSY;
            goto user_register_end;
        }
    }

    resUser->useCount = 0;
    memset(resUser->use, 0, sizeof(struct resourceUser *) * RESOURCE_USER_CNT);
    sUList.push_back(resUser);

user_register_end:
    pthread_mutex_unlock(&sRMutex);

    return ret;
}

int resoucreCoreInit(unsigned int flags)
{
    logInfo(RESOURCE_TAG, "resoucreCoreInit");
    resoucreMutexCheckInit();

    return 0;
}
AIBRAIN_CORE_MODULE_INIT(resoucreCoreInit);