/* add your code here */

#include "IOKitEcho.h"

#define super       IOService


OSDefineMetaClassAndStructors(EchoDriver, IOService);

bool EchoDriver::start(IOService *provider)
{
    if (super::start(provider) != true) {
        return false;
    }
    
    registerService();
    
    LOG("Start EchoDriver... ver %s", OSKextGetCurrentVersionString());
    return true;
}

void EchoDriver::stop(IOService *provider)
{
    LOG("Stop EchoDriver...");
    return super::stop(provider);
}

#undef super