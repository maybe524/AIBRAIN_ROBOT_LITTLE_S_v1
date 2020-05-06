//
// Created by 1 on 2020/5/1.
//

#ifndef EXAMPLE_OBJ_CORE_H
#define EXAMPLE_OBJ_CORE_H

#include <errno.h>

class objOpts {
public:
    virtual int open  (char *objName, unsigned int flags) = 0;
    virtual int read  (void *data, unsigned int len, unsigned int flags) = 0;
    virtual int ctrl  (unsigned int cmd, void *data, unsigned int len, unsigned int flags) = 0;
    virtual int write (void *data, unsigned int len, unsigned int flags) = 0;
    virtual int close (char *objName, unsigned int flags) = 0;
};

#define AIBRAIN_CORE_MODULE_INIT(m)

#endif //EXAMPLE_OBJ_CORE_H
