//
//  log.cc
//  
//
//  Created by Dennis Xu on 10/20/13.
//
//

#include "log.h"
#include <vector>
#include <assert.h>

Log::Log(struct mklfs_opts *opts) {
    this->addr = NULL;
//    this->cp_cur = NULL;
//    this->cp_cache = NULL;
//    this->sp = NULL;
    this->sector_size = FLASH_SECTOR_SIZE;
    this->blk_size = opts->blk_num;
    this->seg_size = opts->segment_num;
    int size = sizeof(struct segment_metadata) + this->seg_size*(sizeof(seg_block));
    int s_size = (size%this->sector_size)? size/this->sector_size+1: size/this->sector_size;
    this->pre_seg_size = (s_size%this->blk_size)? s_size/this->blk_size+1: s_size/this->blk_size;
}

bool Log::open(Flash * flash) {
    assert(flash != NULL);
    this->flash = flash;
    //TODO
    //this->set_superblock(flash);
    //this->set_cp(flash);
    //this->set_logAddress(this->cp_cur);
    
    if (this->addr == NULL) {
        this->addr->seg_num = 1;
        this->addr->blk_num = 0;
    }
    return true;
}
bool Log::close() {
    // TODO
    // update checkpoint
    // write checkpoint to superblock
//    if (Flash_Close(this->flash)) {
//        return TRUE;
//    }
//    else {
//        // print error message;
//        return FALSE;
//    }
    
    return Flash_Close(this->flash);
}

bool Log::Log_Read(struct logAddress *addr, int length, char* buffer) {
    // TODO
    // if the seg
//    if (this->seg_cache->seg_id == addr->seg_num) {
//        
//    }
    u_int sector = addr->seg_num* (this->seg_size*this->blk_size)
                + addr->blk_num * (this->blk_size);
    u_int count = (length%this->sector_size) ? length/this->sector_size+1 : length/this->sector_size;
    return Flash_Read(this->flash, sector, count, buffer);
}

bool Log::Log_Write(int inum, int block, int length, char * buffer, struct logAddress &addr) {
//    int	Flash_Write(Flash flash, u_int sector, u_int count, void *buffer);
    u_int sector = this->addr->seg_num* (this->seg_size*this->blk_size)
                + this->addr->blk_num * (this->blk_size);
    u_int count = (length%this->sector_size)?length/this->sector_size+1:length/this->sector_size;
    // Update the addr for the new file
    addr.seg_num = this->addr->seg_num;
    addr.blk_num = this->addr->blk_num;
    
    //TODO
    // update segment summary table in per-segment metadata
    // update inode version table in superblock
    
    // Update the next segment/block info in log
    this->addr->blk_num++;
    if (this->addr->blk_num >= this->seg_size) {
        this->addr->seg_num++;
        // craate new segment
        //u_int new_segment_sector = this->addr->seg_num* (this->seg_size*this->blk_size);
        //Segment * seg = segment_init(this->flash, this->seg_num, new_segment_sector, this->pre_seg_size);
        //update next
        this->addr->blk_num = this->pre_seg_size;
    }
    
    return Flash_Write(this->flash, sector, count, buffer);
}

//bool Log_Write(inode inode, int block, int length, char * buffer, struct logAddress &addr) {
//}

//bool Log_Free(struct logAddress *addr, int length);
