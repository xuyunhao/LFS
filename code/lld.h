//
//  node.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____lld__
#define ____lld__

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <ctime>

#include "flash.h"

#define MAX_DB	500             // Maximum number of blocks a file  can occupy
#define MAX_FILE_NAME_SIZE 8    // Maximum number of characters for a file name
#define MAXIMUM_BLOCK_NUM 1024  // Maximum number of blocks a segment can contain

#define NULL_INODE -1

#define NULL_BLOCK_INDEX -1


/**
 Struct for opts used in mklfs
 **/
struct mklfs_opts {
    uint32_t segment_num;
    uint32_t sector_num;
    uint32_t blk_num;
    uint32_t wearlimit;
    char* filename;
};


/**
 Struct for Inode
 **/

typedef struct direct_block {
	uint32_t seg_num;
	uint32_t blk_num;
}direct_block;

typedef struct indirct_block {
    uint32_t seg_num;
	uint32_t blk_num;
}indirct_block;

typedef struct inode {
	uint32_t inode_id;	// inode number
	uint32_t size;
    char name[MAX_FILE_NAME_SIZE];   // remove for phase 2
	direct_block direct[MAX_DB];
}inode;

/**
 This is the struct in the flash for
 segment summary table
 **/
typedef struct seg_block {
    uint32_t block_id;
    uint32_t inode_id;
    time_t inode_v;
    uint32_t current_usage;
} seg_block;

/**
 This is the struct in the flash for
 other metadata in the pre-segment
 block
 **/
typedef struct segment_metadata {
    uint32_t seg_id;
	uint32_t inode_num;      // inode number of the file
	uint32_t logical_blk;	// logical block number of the file
    uint32_t current_usage;  // the wearlimit usage
    time_t version;        // this can change to timestamp
    
    int64_t remaining_size; // initial to the maximum size
    //    seg_block * block_list;
    int     prev_start_sector, next_start_sector;
}segment_metadata;

typedef struct superblock {
    int sector_size;
    int sector_per_blk;
    int blk_per_seg;
    int wearlimit;
    int current_usage;
    int num_seg_sp;
    int total_num_seg;
}superblock;

typedef struct checkpoint {
    time_t  version;
    int     tail_seg_id;
    int     tail_blk_id;
    
    int     ifile_inode_seg_id;
    int     ifile_inode_blk_id;
} checkpoint;

typedef struct logAddress {
    uint32_t seg_num;
    uint32_t blk_num;
}logAddress;




#endif /* defined(____lld__) */
