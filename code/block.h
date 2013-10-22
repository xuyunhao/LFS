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
    int current_usage;
} seg_block;

class Block {
public:
    Block(Flash * flash, int start_sector_id, int sector_size, int blk_id, int wearlimit, int current_usage);
    ~Block();
    
    int get_block_id();
    int get_inode_id();
    int get_inode_v();
    char * get_content();
    bool is_used();

    int get_current_usage();
    int get_remaining_usage();
    bool update_usage();
    
    bool set_content(char * s, int inode_id, int inode_v);
    
    bool read_block(char * metadata, char * block_contents, int pre_seg_size);
    char * to_string();
private:
    Flash * flash;
    int blk_id;
    int inode_id;
    int inode_v;

    // the usage of wearlimit
    int current_usage;
    int wearlimit;
    
    int sector_offset;  // offset of the starting sector
    int sector_size;    // the number of sectors in block

    char* content;      // the content in the block
};

inline int Block::get_block_id() {return this->blk_id;}
inline int Block::get_inode_id() {return this->inode_id;}
inline int Block::get_inode_v() {return this->inode_v;}
inline char * Block::get_content() {return this->content;}
inline bool Block::is_used() {return this->content != NULL;}
inline int Block::get_current_usage() {return this->current_usage;}
inline int Block::get_remaining_usage() {return this->wearlimit - this->current_usage;}
inline bool Block::update_usage() {this->current_usage++; return this->current_usage < this->wearlimit;}

#endif /* defined(____block__) */
