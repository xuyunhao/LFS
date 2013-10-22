//
//  superblock.cc
//  
//
//  Created by Dennis Xu on 10/22/13.
//
//

#include "superblock.h"

SuperBlock::SuperBlock(Flash * flash, Log * log) {
    this->log = log;
    this->flash = flash;
    // Read from flash
//    this->sector_size = sector_size;
//    this->sector_per_blk = sector_per_blk;
//    this->blk_per_seg = blk_per_seg;
//    this->wearlimit = wearlimit;
//    this->current_usage = 1;
}
SuperBlock::SuperBlock(Flash * flash, Log * log, int sector_size, int sector_per_blk, int blk_per_seg, int wearlimit) {
    this->log = log;
    this->flash = flash;
    this->sector_size = sector_size;
    this->sector_per_blk = sector_per_blk;
    this->blk_per_seg = blk_per_seg;
    this->wearlimit = wearlimit;
    this->current_usage = 1;
}

CheckPoint * SuperBlock::get_most_recent_checkpoint() {
    return (this->cp_list).back();
}

bool SuperBlock::create_new_checkpoint();

bool SuperBlock::write_to_flash();
