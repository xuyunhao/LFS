//
//  inode.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____inode__
#define ____inode__

#include <"lld.h">

struct direct_block {
	int32_t seg_num;
	int32_t blk_num;
};
struct indirct_block {
    int32_t seg_num;
	int32_t blk_num;
    struct inode;
};
struct inode {
	uint32_t inode_id;	// inode number
	uint32_t size;
    char16_t name[MAX_FILE_NAME_SIZE];   // remove for phase 2
	struct direct_block direct[MAX_DB];
};

#endif /* defined(____inode__) */
