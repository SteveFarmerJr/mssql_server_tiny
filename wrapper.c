#define _GNU_SOURCE
#include <sys/sysinfo.h>
#include <dlfcn.h>
#include <stdio.h>

int sysinfo(struct sysinfo *info){
    // clear it
    //dlerror();
    void *pt=NULL;
    typedef int (*real_sysinfo)(struct sysinfo *info);

    // we need the real sysinfo function address
    pt = dlsym(RTLD_NEXT,"sysinfo");
    //printf("pt: %x\n", *(char *)pt);

    // call the real sysinfo system call
    int real_return_val=((real_sysinfo)pt)(info);

    // but then modify its returned totalram field if necessary
    // because sqlserver needs to believe it has "4000 megabytes"
    // physical memory
    if( info->totalram < 1000l * 1000l * 1000l * 4l){ // 4000 megabytes
        info->totalram = 1000l * 1000l * 1000l * 4l ;
    }

    return real_return_val;
}

