//
//  EchoDriverClient.cpp
//  Echo
//
//  Created by Kent Huang on 2015/5/14.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#include "EchoDriverClient.h"

#define super IOUserClient

OSDefineMetaClassAndStructors(EchoDriverClient,IOUserClient);

bool EchoDriverClient::initWithTask(task_t owningTask, void *securityID, UInt32 type)
{
    if (clientHasPrivilege(owningTask, kIOClientPrivilegeAdministrator) != KERN_SUCCESS) {
        LOGE("Unprivileged client attempted to connect.");
        return false;
    }
    
    return super::initWithTask(owningTask, securityID, type);
}

bool EchoDriverClient::start(IOService *provider)
{
    if (super::start(provider) == false) {
        return false;
    }
    LOG("Start client...");
    
    return true;
}

void EchoDriverClient::stop(IOService *provider)
{
    LOG("Stop client...");
    return super::stop(provider);
}

#undef super