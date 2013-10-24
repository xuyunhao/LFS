//
//  inode.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "inode.h"
#include <cstring>

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
            itr->second = *addr;
            is_updated = true;
            break;
        }
    }
    if (!is_updated) {
        (this->blk_map)[offset] = *addr;
    }
    return true;
}
