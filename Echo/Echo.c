//
//  Echo.c
//  Echo
//
//  Created by Kent Huang on 2015/4/30.
//  Copyright (c) 2015年 KentHuang. All rights reserved.
//

#include <mach/mach_types.h>

kern_return_t Echo_start(kmod_info_t * ki, void *d);
kern_return_t Echo_stop(kmod_info_t *ki, void *d);

kern_return_t Echo_start(kmod_info_t * ki, void *d)
{
    return KERN_SUCCESS;
}

kern_return_t Echo_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
