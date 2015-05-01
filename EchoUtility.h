//
//  EchoUtility.h
//  Echo
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#ifndef __Echo__EchoUtility__
#define __Echo__EchoUtility__

#include <libkern/libkern.h>
#include <sys/proc.h>

void PrintCurrentProcess(void);

#ifdef KEXT

#define LOG(...)    PrintCurrentProcess();printf("Echo: " __VA_ARGS__); printf("\n");
#define LOGE(...)   PrintCurrentProcess;printf("Echo: [Error] " __VA_ARGS__); printf("\n");


#elif IOKIT_KEXT

#include <IOKit/IOLib.h>

#define LOG(...)    PrintCurrentProcess;IOLog("Echo: " __VA_ARGS__); IOLog("\n");
#define LOGE(...)   PrintCurrentProcess;IOLog("Echo: [Error] " __VA_ARGS__); IOLog("\n");

#endif



#endif /* defined(__Echo__EchoUtility__) */
