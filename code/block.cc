//
//  block.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "block.h"
#include "flash.h"
#include <string.h>
#include <stdlib.h>

Block::Block(Flash * flash, int start_sector_id, int sector_size, int blk_id) {
    this->flash = flash;
    this->sector_offset = start_sector_id;
    this->sector_size = sector_size;
    this->blk_id = blk_id;
    
    this->inode_id = NULL_INODE;
    this->inode_v = NULL_INODE;
    
    this->content = NULL;
}

Block::~Block() {
    delete this->content;
}

bool Block::set_content(char * s, int inode_id, int inode_v) {
    this->inode_id = inode_id;
    this->inode_v = inode_v;
    memcpy(this->content, s, strlen(s));
    
    return (this->content != NULL);
}

bool Block::read_block(char * metadata, char * block_contents, int pre_seg_size) {
    seg_block * sb = (seg_block *)metadata;
    this->blk_id = sb->block_id;
    this->inode_id = sb->inode_id;
    this->inode_v = sb->inode_v;
    
    int len = this->sector_size * FLASH_SECTOR_SIZE;
    int offset = len * (this->blk_id - pre_seg_size);
    memcpy(this->content, block_contents, len);
    
    return (this->content != NULL);
}

char * Block::to_string() {
    char * element_in_blk_list = (char *) malloc(sizeof(seg_block));
    ((seg_block *) element_in_blk_list)->block_id = this->blk_id;
    ((seg_block *) element_in_blk_list)->inode_id = this->inode_id;
    ((seg_block *) element_in_blk_list)->inode_v = this->inode_v;
    return element_in_blk_list;
}