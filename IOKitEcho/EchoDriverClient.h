//
//  EchoDriverClient.h
//  Echo
//
//  Created by Kent Huang on 2015/5/14.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#ifndef __Echo__EchoDriverClient__
#define __Echo__EchoDriverClient__
#include <IOKit/IOUserClient.h>

#include "EchoUtility.h"

#endif /* defined(__Echo__EchoDriverClient__) */

class EchoDriverClient : public IOUserClient
{
    OSDeclareDefaultStructors(EchoDriverClient);
public:
    bool initWithTask(task_t owningTask, void *securityID, UInt32 type);
    bool start(IOService *provider);
    void stop(IOService *provider);
private:
};