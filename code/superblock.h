//
//  superblock.h
//  
//
//  Created by Dennis Xu on 10/22/13.
//
//

#ifndef ____superblock__
#define ____superblock__

#include <iostream>
#include <vector>
#include "lld.h"
#include "log.h"
#include "checkpoint.h"

typedef std::vector<CheckPoint> cp_list_type;
typedef std::vector<CheckPoint>::iterator cp_list_itr;

class SuperBlock {
public:
    SuperBlock(Flash * flash, Log * log);
    SuperBlock(Flash * flash, Log * log, int sector_size, int sector_per_blk, int blk_per_seg, int total_sector, int wearlimit);
    ~SuperBlock();
    
    CheckPoint * get_most_recent_checkpoint();
    
    bool create_new_checkpoint();
    
    bool write_to_flash();
    
    int get_size();
private:
    cp_list_type cp_list;
    
    Log * log;
    Flash * flash;
    int size;       // number of segments for superblock
                    // compute in the constructor
    int sector_size;
    int sector_per_blk;
    int blk_per_seg;
  
    int total_num_seg;
    
    int wearlimit;
    int current_usage;
};

inline int SuperBlock::get_size() { return this->size;}

#endif /* defined(____superblock__) */
