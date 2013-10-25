//
//  file.cc
//  
//
//  Created by Dennis Xu on 10/24/13.
//
//

#include "file.h"

File::File(Inode * inode, Log* log) {
    this->node = inode;
    this->lg = log;
}

bool File::File_Read(int off, int len, char * buf) {
    int inum = this->node->get_id();
    int blk_size = this->lg->get_block_size();
    int block = off;
    if (len == 0) {
        return true;
    }
    addr_list list = (this->node->get_blk_list());
    for (int r = len; r >= 0; r -= blk_size) {
        logAddress * la = list[block];
        int s = (r<len)? r:len;
        if (!(this->lg->Log_Read(la,s, buf))) {
            return false;
        }
        block++;
    }
    return true;
}

bool File::File_Write(int len, char * buf) {
    int inum = this->node->get_id();
    int blk_size = this->lg->get_block_size();
    int block = 0;
    if (len == 0) {
        return true;
    }
    for (int r = len; r >= 0; r -= blk_size) {
        logAddress * la;
        int s = (r<len)? r:len;
        if (!(this->lg->Log_Write(inum, block, s, buf, *la))) {
            return false;
        }
        this->node->update(block, la);
        block++;
    }
    return true;
}
