//
//  mklfs.h
//  
//
//  Created by Dennis Xu on 10/18/13.
//
//

#ifndef ____mklfs__
#define ____mklfs__

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

struct mklfs_opts {
    u_int segment_num;
    u_int sector_num;
    u_int blk_num;
    u_int wearlimit;
    char* filename;
};

#endif /* defined(____mklfs__) */
