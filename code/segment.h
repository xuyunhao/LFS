//
//  segment.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____segment__
#define ____segment__

// #include <"block.h">
#include <"lld.h">

struct seg_block {
    int32_t block_id;
    int32_t inode_id;
    int32_t inode_v;
};s

struct segment {
	int32_t inode_num;      // inode number of the file
	int32_t logical_blk;	// logical block number of the file
    
    int64_t version;        // this can change to timestamp
    
    int64_t remaining_size; // initial to the maximum size

    struct seg_block [] block_list;
    struct segment* prev, next;
};


#endif /* defined(____segment__) */
