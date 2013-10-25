//
//  inode.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "inode.h"
#include <cstring>
#include <stdlib.h>

Inode::Inode(){}
Inode::Inode(Flash flash, int node_id, char fileaname[]) {
    this->flash = flash;
    this->node_id = node_id;
    strcpy(this->filename, filename);
    time(&version);
}

Inode::~Inode() {
    this->blk_map.clear();
}

addr_list Inode::get_blk_list() {
    addr_list list;
    for (addr_map_itr itr = (this->blk_map).begin();
         itr != (this->blk_map).end(); itr++) {
        list.push_back(itr->second);
    }
    return list;
}

bool Inode::update(int offset, logAddress * addr){
    bool is_updated = false;
    for (addr_map_itr itr = (this->blk_map).begin();
         itr != (this->blk_map).end(); itr++) {
        if (offset < itr->first) {
            itr->second = addr;
            is_updated = true;
            break;
        }
    }
    if (!is_updated && (this->blk_map).size() < 4) {
        (this->blk_map)[offset] = addr;
        return true;
    }
    return false;
}

char * Inode::to_string() {
    char * str = (char *) malloc(MAX_BLOCK_PER_FILE * sizeof(direct_block)+sizeof(inode));
    inode * in = (inode *) str;
    direct_block * db = (direct_block *)(str + sizeof(inode));
    int i = 0;
    in->inode_id = this->node_id;
    memcpy(in->name, this->filename, MAX_FILE_NAME_SIZE);
    in->version = this->version;
    in->size = ((this->blk_map).size() < MAX_BLOCK_PER_FILE)?(this->blk_map).size():MAX_BLOCK_PER_FILE;
    for (addr_map_itr itr = (this->blk_map).begin();
         i < MAX_BLOCK_PER_FILE && itr != (this->blk_map).end();
         itr++, i++)
    {
        db->seg_num = itr->second->seg_num;
        db->blk_num = itr->second->blk_num;
        db += sizeof(direct_block);
    }
    return str;
}
