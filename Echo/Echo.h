//
//  Echo.h
//  Echo
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#ifndef Echo_Echo_h
#define Echo_Echo_h

#include <mach/mach_types.h>
#include <libkern/OSAtomic.h>
#include <libkern/OSMalloc.h>
#include <libkern/libkern.h>

#include <sys/sysctl.h>

#include "EchoUtility.h"

#define ECHO_KEXT_NAME "org.debugkernel.Echo"
#define ECHO_OS_MALLOC_TAG  'ohcE'

OSMallocTag gMallocTag = 0;
unsigned int gSysctlCmd = 0;
unsigned int gPingCounter = 0;

#endif
