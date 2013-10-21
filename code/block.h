//
//  block.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____block__
#define ____block__

#include "flash.h"
#include <stdlib.h>

#define NULL_INODE -1
/**
 This is the struct in the flash for
 segment summary table
 **/
typedef struct seg_block {
    int block_id;
    int inode_id;
    int inode_v;
} seg_block;

class Block {
public:
    Block(Flash * flash, int start_sector_id, int sector_size, int blk_id);
    ~Block();
    
    int get_block_id();
    int get_inode_id();
    int get_inode_v();
    char * get_content();
    bool is_used();

    bool set_content(char * s, int inode_id, int inode_v);
    
    bool read_block(char * metadata, char * block_contents, int pre_seg_size);
    char * to_string();
private:
    Flash * flash;
    int blk_id;
    int inode_id;
    int inode_v;
    
    int sector_offset;  // offset of the starting sector
    int sector_size;    // the number of sectors in block

    char* content;      // the content in the block
};

inline int Block::get_block_id() {return this->blk_id;}
inline int Block::get_inode_id() {return this->inode_id;}
inline int Block::get_inode_v() {return this->inode_v;}
inline char * Block::get_content() {return this->content;}
inline bool Block::is_used() {return this->content != NULL;}


#endif /* defined(____block__) */
