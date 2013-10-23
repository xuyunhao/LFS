//
//  log.h
//  
//
//  Created by Dennis Xu on 10/20/13.
//
//

#ifndef ____log__
#define ____log__

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "flash.h"
#include "mklfs.h"
#include "segment.h"

typedef struct logAddress {
    int seg_num;
    int blk_num;
}logAddress;

class Log {
public:
    Log(struct mklfs_opts *opts);
    ~Log();

    bool open(Flash *flash);
    bool close();
    
    bool Log_Read(struct logAddress * addr, int length, char* buffer);
    bool Log_Write(int inum, int block, int length, char * buffer, struct logAddress &addr);
    bool Log_Free(struct logAddress * addr, int length);

    logAddress * getLogAddress();
    int get_total_sectors();
// TODO
//    Superblock * superblock();
//    Checkpoint * Checkpoint();

    
private:
    struct logAddress* addr;
    Flash* flash;
    int sector_size;    // the sector size in byte
    int blk_size;       // the num in sectors for a block
    int seg_size;       // the num in block for a segment
    int pre_seg_size;   // the num in block for pre-segment metedata
    int total_sector;    // total
// TODO
    Segment * seg_cache;
//    struct Checkpoint cp_cache;
//    struct Checkpoint cp_cur;
//    struct Superblock sp;
};
inline logAddress * Log::getLogAddress() {return this->addr;}
inline int Log::get_total_sectors() {return this->total_sector;}

#endif /* defined(____log__) */
