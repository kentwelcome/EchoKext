//
//  EchoUtility.c
//  Echo
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#include "EchoUtility.h"

void PrintCurrentProcess(void)
{
    char procName[256] = {0};
    proc_selfname(procName, sizeof(procName));
#ifdef KEXT
    printf("%s(%x) ",procName,proc_selfpid());
#elif IOKIT_KEXT
    IOLog("%s(%x) ",procName,proc_selfpid());
#endif
}
