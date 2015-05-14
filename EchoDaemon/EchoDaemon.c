//
//  EchoDaemon.c
//  EchoDaemon
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#include "EchoDaemon.h"
const char *extractFileName(const char *fullPath)
{
    const char *ptr = NULL;
    size_t len = strlen(fullPath);
    ptr = &fullPath[len];
    
    for (size_t i = len; i > 0; i--) {
        if (fullPath[i] == '/') {
            ptr = &fullPath[i+1];
            break;
        }
    }
    
    return ptr;
}

void ShowUsage(const char *deamonName)
{
    printf("Usage: %s id\n",deamonName);
}

int fd = 0;

void disconnectWithKernel()
{
    printf("[\033[0;32mo\033[0m] Disconnect to kernel...\n");
    shutdown(fd, SHUT_RDWR);
}

int main(int argc, const char * argv[]) {
    int id = 0;
    int rc = 0;
    int terminate = 0;
    struct sockaddr_ctl addr;
    ssize_t  recvSize = 0;
    ECHO_CMD sendData;
    ECHO_CMD recvData;
    
    // Init
    bzero(&addr, sizeof(addr));
    addr.sc_len     = sizeof(addr);
    addr.sc_family  = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_id      = 0;
    addr.sc_unit    = 0;
    
    if (argc != 2) {
        ShowUsage(extractFileName(argv[0]));
        return -1;
    }
    id = atoi(argv[1]);
    addr.sc_id = id;
    
    fd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (fd == -1) {
        printf("[\033[0;31mx\033[0m] Fail to create socket\n");
        return -1;
    }
    printf("[\033[0;32mo\033[0m] Create socket... fd=%d\n",fd);
    
    rc = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc) {
        printf("[\033[0;31mx\033[0m] Fail to connect to kernel... rc=%d\n",rc);
        return -1;
    }
    
    printf("[\033[0;32mo\033[0m] Connect to kernel... rc=%d\n",rc);
    
    rc = setsockopt(fd, SYSPROTO_CONTROL, 123, NULL, 0);
    if (rc) {
        printf("[\033[0;31mx\033[0m] Fail to send command to kernel... rc=%d\n",rc);
    }
    printf("[\033[0;32mo\033[0m] Send command to kernel... rc=%d\n",rc);
    
    signal(SIGTERM, disconnectWithKernel);
    signal(SIGINT, disconnectWithKernel);
    
    while (terminate == 0) {
        recvSize = recv(fd, &recvData, sizeof(recvData), 0);
        if (recvSize > 0) {
            printf(" <recv> CMD: %x Data: %s\n",recvData.cmd_type,recvData.data);
            if (recvData.cmd_type == EchoCmdQuit) {
                terminate = 1;
            }
        }
        else {
            printf("    Disconnect send from kernel...\n");
            terminate = 1;
        }
    }
    
    shutdown(fd, SHUT_RDWR);
    printf("[\033[0;32mo\033[0m] Disconnect to kernel...\n");    
    
    return 0;
}
