//
//  EchoKernelControl.h
//  Echo
//
//  Created by Kent Huang on 2015/5/1.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#ifndef Echo_EchoKernelControl_h
#define Echo_EchoKernelControl_h

#define EXHO_CTL_NAME   "org.debugkernel.kerctl"

#define ECHO_ID     0
#define ECHO_UNIT   0

#define ECHO_CMD_INIT   0
#define ECHO_CMD_MSG    1
#define ECHO_CMD_QUIT   2

typedef enum {
    EchoCmdInit = 0,
    EchoCmdMsg,
    EchoCmdQuit
} ECHO_CMD_TYPE;

typedef struct {
    unsigned int    size;
    ECHO_CMD_TYPE   cmd_type;
    char            data[256];
} ECHO_CMD, *PECHO_CMD;

#endif
