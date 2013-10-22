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

typedef struct superblock {
    int sector_size;
    int sector_per_blk;
    int blk_per_seg;
    int wearlimit;
    int current_usage;
    int num_seg_sp;
    int total_num_seg;
}superblock;

class SuperBlock {
public:
    SuperBlock(Flash * flash, Log * log);
    SuperBlock(Flash * flash, Log * log, int sector_size, int sector_per_blk, int blk_per_seg, int wearlimit);
    ~SuperBlock();
    
    CheckPoint * get_most_recent_checkpoint();
    
    bool create_new_checkpoint();
    
    bool write_to_flash();
    
    int get_size();
private:
    std::vector<CheckPoint> cp_list;
    
    Log * log;
    Flash * flash;
    int size;       // number of segments for superblock
                    // compute in the constructor
    int sector_size;
    int sector_per_blk;
    int blk_per_seg;
  
    int wearlimit;
    int current_usage;
};
#endif /* defined(____superblock__) */
