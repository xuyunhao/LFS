//
//  superblock.cc
//  
//
//  Created by Dennis Xu on 10/22/13.
//
//

#include "superblock.h"
#include <cstring>

SuperBlock::SuperBlock(Flash flash, Log * log) {
    this->log = log;
    this->flash = flash;
    // Read from flash
//    this->sector_size = sector_size;
//    this->sector_per_blk = sector_per_blk;
//    this->blk_per_seg = blk_per_seg;
//    this->wearlimit = wearlimit;
//    this->current_usage = 1;
//    this->size = 1;
}
SuperBlock::SuperBlock(Flash flash, Log * log, int sector_size, int sector_per_blk, int blk_per_seg, int total_sector, int wearlimit) {
    this->log = log;
    this->flash = flash;
    this->sector_size = sector_size;
    this->sector_per_blk = sector_per_blk;
    this->blk_per_seg = blk_per_seg;
    this->wearlimit = wearlimit;
    this->current_usage = 1;
    this->total_num_seg = total_sector/(this->sector_per_blk * this->blk_per_seg);
    
    // estimate the size of a superblock
    int size_of_segment =sector_size * this->sector_per_blk * this->blk_per_seg;
    int num_sector = log->get_total_sectors();
    int num_seg = num_sector/(this->sector_per_blk * this->blk_per_seg);
    int num_blk = num_sector/this->sector_per_blk;
    // assume the worst case:
    // each file has one block
    int estimate_size = sizeof(superblock)
                + 10 * (sizeof(checkpoint)
//                        + num_blk * (sizeof(inode_version_table))
//                        + num_seg * (sizeof(seg_usage_table))
                        );
    if (estimate_size < size_of_segment) {
        this->size = 1;
    }
    else {
        this->size = (estimate_size%size_of_segment)? estimate_size/size_of_segment+1 : estimate_size/size_of_segment;
    }
}

SuperBlock::~SuperBlock() {}

CheckPoint * SuperBlock::get_most_recent_checkpoint() {
    return (this->cp_list).back();
}

bool SuperBlock::create_new_checkpoint() {
    CheckPoint * cp = new CheckPoint(this->log);
    int size = (this->cp_list).size();
    (this->cp_list).push_back(cp);
    return cp != NULL && (this->cp_list).size()-size == 1;
}

bool SuperBlock::write_to_flash() {
    int cp_len = this->cp_list.size();
    int seg_size_byte = sector_size * this->sector_per_blk * this->blk_per_seg;
    char * str = (char *) malloc(seg_size_byte);
    char * cp = str + sizeof(superblock);
    
    int remaining_byte = seg_size_byte - sizeof(superblock);
    int seg_id = 0;

    // write default metadata to flash
    superblock * sp = (superblock *) str;
    sp->sector_size = this->sector_size;
    sp->sector_per_blk = this->sector_per_blk;
    sp->blk_per_seg = this->blk_per_seg;
    sp->wearlimit = this->wearlimit;
    sp->current_usage = this->current_usage;
    sp->num_seg_sp = this->size;
    sp->total_num_seg = this->total_num_seg;
    
    for(cp_list_itr iterator = this->cp_list.begin();
        iterator != this->cp_list.end();
        iterator++)
    {
        char * cp_str = (*iterator)->to_string();
        int l = strlen(cp_str);
        if (remaining_byte > l) {
            memcpy(cp, cp_str, l);
            cp += l;
            remaining_byte -= l;
        }
        else {
            // write current cp to flash
            int sector_id = seg_id * this->sector_per_blk * this->blk_per_seg;
            if(!Flash_Write(this->flash, sector_id,
                            this->sector_per_blk * this->blk_per_seg, str)) {
                return false;
            }
            // get a new segment
            seg_id++;
            // then prepare content in new segment
            str = (char *) malloc(seg_size_byte);
            cp = str;
            memcpy(cp, cp_str, l);
            remaining_byte = seg_size_byte - l;
            cp += l;
        }
    }
    int sector_id = seg_id * this->sector_per_blk * this->blk_per_seg;
    return Flash_Write(this->flash, sector_id,
                this->sector_per_blk * this->blk_per_seg, str);
}
