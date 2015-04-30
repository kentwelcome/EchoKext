//
//  Echo.c
//  Echo
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#include "Echo.h"

static int SysctlPingFromDaemonHandler(
                                       struct sysctl_oid * oidp,
                                       void *              arg1,
                                       int                 arg2,
                                       struct sysctl_req * req
                                       )
{
    int rc = 0;
    
    rc = sysctl_handle_long(oidp, arg1, arg2, req);
    
    if (rc == 0 && req->newptr != 0) {
        OSIncrementAtomic(&gPingCounter);
        LOG("Ping from daemon... %d times",gPingCounter);
    }
    
    return rc;
}

static int SysctlResetPingCounter(
                                  struct sysctl_oid * oidp,
                                  void *              arg1,
                                  int                 arg2,
                                  struct sysctl_req * req
                                  )
{
    int rc = 0;
    
    rc = sysctl_handle_long(oidp, arg1, arg2, req);
    
    if (rc == 0 && req->newptr != 0) {
        OSBitAndAtomic(0,&gPingCounter);
        LOG("Reset ping counter... %d times",gPingCounter);
    }
    
    return rc;
}

SYSCTL_OID(
           _kern,
           OID_AUTO,
           echokext_ping_from_daemon,
           CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_KERN,
           &gSysctlCmd,
           sizeof(gSysctlCmd),
           SysctlPingFromDaemonHandler,
           "UI",
           "Ping Echo kext from daemon"
           );

SYSCTL_OID(
           _kern,
           OID_AUTO,
           echokext_reset_ping_counter,
           CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_KERN,
           &gSysctlCmd,
           sizeof(gSysctlCmd),
           SysctlResetPingCounter,
           "UI",
           "Reset ping counter"
           );


static struct sysctl_oid *sysctlHandlerTable[] = {
    &sysctl__kern_echokext_ping_from_daemon,
    &sysctl__kern_echokext_reset_ping_counter,
    NULL
};

kern_return_t RegisterSysctlInterfaces()
{
    struct sysctl_oid *oidp = NULL;
    int i = 0;
    
    LOG("RegisterSysctlInterfaces: Start to register sysctl\n");
    
    oidp = sysctlHandlerTable[i];
    while (oidp != NULL) {
        LOG("[Add] sysctlHandlerTable[%d]: %s\n",i,oidp->oid_name);
        sysctl_register_oid(oidp);
        oidp = sysctlHandlerTable[++i];
    }
    
    return KERN_SUCCESS;
}

kern_return_t UnregisterSysctlInterfaces()
{
    struct sysctl_oid *oidp = NULL;
    int i = 0;
    LOG("UnregisterSysctlInterfaces: Start to unregister sysctl\n");
    
    oidp = sysctlHandlerTable[i];
    while (oidp != NULL) {
        LOG("[Remove] sysctlHandlerTable[%d]: %s\n",i,oidp->oid_name);
        sysctl_unregister_oid(oidp);
        oidp = sysctlHandlerTable[++i];
    }
    
    return KERN_SUCCESS;
}

kern_return_t Echo_start(kmod_info_t * ki, void *d);
kern_return_t Echo_stop(kmod_info_t *ki, void *d);

kern_return_t Echo_start(kmod_info_t * ki, void *d)
{
    LOG("Start kext...");
    kern_return_t status = KERN_SUCCESS;
    gMallocTag = OSMalloc_Tagalloc(ECHO_KEXT_NAME, ECHO_OS_MALLOC_TAG);
    if (gMallocTag == 0) {
        LOGE("Fail to allocate MallocTag");
        status = KERN_FAILURE;
    }
    
    if (status == KERN_SUCCESS) {
        RegisterSysctlInterfaces();
    }
    
    if (status != KERN_SUCCESS) {
        LOGE("Fail to start kext");
        Echo_stop(ki, d);
    }
    return KERN_SUCCESS;
}

kern_return_t Echo_stop(kmod_info_t *ki, void *d)
{
    LOG("Stop kext..");
    
    UnregisterSysctlInterfaces();
    
    if (gMallocTag != 0) {
        OSMalloc_Tagfree(gMallocTag);
        gMallocTag = 0;
    }
    
    return KERN_SUCCESS;
}
