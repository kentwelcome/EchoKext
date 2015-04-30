//
//  EchoUtility.h
//  Echo
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#ifndef __Echo__EchoUtility__
#define __Echo__EchoUtility__

#ifdef KEXT

#define LOG(...)    printf("Echo: " __VA_ARGS__); printf("\n");
#define LOGE(...)   printf("Echo: [Error] " __VA_ARGS__); printf("\n");


#elif IOKIT_KEXT

#include <IOKit/IOLib.h>

#define LOG(...)    IOLog("Echo: " __VA_ARGS__); IOLog("\n");
#define LOGE(...)   IOLog("Echo: [Error] " __VA_ARGS__); IOLog("\n");

#endif

#endif /* defined(__Echo__EchoUtility__) */
