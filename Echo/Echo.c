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
    ECHO_CMD cmd = {0};
    
    rc = sysctl_handle_long(oidp, arg1, arg2, req);
    
    if (rc == 0 && req->newptr != 0) {
        OSIncrementAtomic(&gPingCounter);
        LOG("Ping from daemon... %d times",gPingCounter);
        if (gKeCtlConnected == 1) {
            LOG("Send from kernel...");
            cmd.size = sizeof(cmd);
            cmd.cmd_type = EchoCmdMsg;
            strncpy(cmd.data, "send from kernel", strlen("send from kernel"));
            ctl_enqueuedata(gKeCtlRef,gKeCtlSacUnit,&cmd,sizeof(cmd),0);
        }
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

#pragma mark -

errno_t KeCtlSetHandler( kern_ctl_ref ctlref, unsigned int unit, void *userdata, int opt, void *data, size_t len )
{
    LOG(">>> KeCtlSetHandler");
    return KERN_SUCCESS;
}

errno_t KeCtlGetHandler(kern_ctl_ref ctlref, unsigned int unit, void *userdata, int opt, void *data, size_t *len)
{
    LOG(">>> KeCtlGetHandler");
    return KERN_SUCCESS;
}

errno_t KeCtlConnectHandler(kern_ctl_ref ctlref, struct sockaddr_ctl *sac, void **unitinfo)
{
    LOG(">>> KeCtlConnectionHandler");
    gKeCtlConnected = 1;
    gKeCtlSacUnit = sac->sc_unit;
    
    return KERN_SUCCESS;
}

errno_t KeCtlDisconnectHandler(kern_ctl_ref ctlref, unsigned int unit, void *unitinfo)
{
    LOG(">>> LeCtlDisconnectHandler");
    gKeCtlConnected = 0;
    gKeCtlSacUnit = 0;
    return KERN_SUCCESS;
}

errno_t KeCtlSendHandler(kern_ctl_ref ctlref, unsigned int unit, void *userdata, mbuf_t m, int flags)
{
    LOG(">>> KeCtlSendHandler");
    return KERN_SUCCESS;
}



void InitializateKernCtlConnection()
{
    errno_t err;
    LOG("Initializate KerCtlConnection...");
    
    bzero(&gKeCtlReg, sizeof(KERN_CTL_REG));
    gKeCtlReg.ctl_id    = ECHO_ID;
    gKeCtlReg.ctl_unit  = ECHO_UNIT;
    strncpy( gKeCtlReg.ctl_name, EXHO_CTL_NAME, strlen(EXHO_CTL_NAME));
    gKeCtlReg.ctl_flags         = CTL_FLAG_PRIVILEGED & CTL_FLAG_REG_ID_UNIT;
    gKeCtlReg.ctl_send          = KeCtlSendHandler;
    gKeCtlReg.ctl_getopt        = KeCtlGetHandler;
    gKeCtlReg.ctl_setopt        = KeCtlSetHandler;
    gKeCtlReg.ctl_connect       = KeCtlConnectHandler;
    gKeCtlReg.ctl_disconnect    = KeCtlDisconnectHandler;
    
    err = ctl_register(&gKeCtlReg, &gKeCtlRef);
    if (err == KERN_SUCCESS) {
        LOG("Register KerCtlConnection success: id=%d",gKeCtlReg.ctl_id);
    }
    else {
        LOG("Fail to register: err=%d",err);
    }
}

void DeinitializateKernCtlConnection()
{
    errno_t err;
    LOG("Deinitializate KernCtlConnection...");
    err = ctl_deregister(gKeCtlRef);
    if (err == 0) {
        gKeCtlRef = NULL;
    }
    else {
        LOG("Fail to deregister: err=%d",err);
    }
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
    
    if (status == KERN_SUCCESS) {
        InitializateKernCtlConnection();
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
    
    if (gKeCtlRef != NULL) {
        DeinitializateKernCtlConnection();
    }
    
    UnregisterSysctlInterfaces();
    
    if (gMallocTag != 0) {
        OSMalloc_Tagfree(gMallocTag);
        gMallocTag = 0;
    }
    
    return KERN_SUCCESS;
}
