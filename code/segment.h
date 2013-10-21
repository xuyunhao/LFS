//
//  segment.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____segment__
#define ____segment__

#include "block.h"
#include "lld.h"
#include <stdlib.h>
#include <vector>
/**
 This is the struct in the flash for
 other metadata in the pre-segment 
 block
 **/
struct segment_metadata {
    int32_t seg_id;
	int32_t inode_num;      // inode number of the file
	int32_t logical_blk;	// logical block number of the file
    
    time_t version;        // this can change to timestamp
    
    int64_t remaining_size; // initial to the maximum size
    //    seg_block * block_list;
    int     prev_start_sector, next_start_sector;
};


class Segment {
public:
    Segment(Flash * flash, int seg_id, int start_sector_id, int pre_seg_size, int blk_per_seg, int sector_per_blk, Segment prev);
    Segment(Flash *flash, int start_sector_id, int pre_seg_size, int blk_per_seg, int sector_per_blk);
    ~Segment();
    
    inline int get_seg_id();
    inline int get_start_sector();
    inline int get_reamaining_blks();
    inline int get_next_free_block_id();
    
    Segment * get_prev();
    Segment * get_next();
    Block * get_blk(int blk_id);
    
    bool set_new_blk(int inode_id, int inode_v, char * s);
    bool write_to_flash();
    bool read_from_flash();
private:
    Flash * flash;
    int seg_id;
    int sector_offset;
    int pre_seg_size;
    int sector_per_blk;
    int next_free_blk_id;
    int blk_per_seg;
    
    int prev_seg_starting_sector;
    int next_seg_starting_sector;
    
	int32_t logical_blk;	// logical block number of the file
    time_t version;        // this can change to timestamp
    int64_t remaining_size; // number of remaining blk
                            // initial to the maximum size
    int64_t maximum_size;   // number of the maximum blk
    std::vector<Block> blk_list;
};

inline int Segment::get_seg_id() { return this->seg_id;}
inline int Segment::get_start_sector() { return this->sector_offset;}
inline int Segment::get_reamaining_blks() { return this->remaining_size;}
inline int Segment::get_next_free_block_id() {return this->next_free_blk_id;}

#endif /* defined(____segment__) */
