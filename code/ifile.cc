//
//  ifile.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "ifile.h"

Ifile::Ifile(Flash flash){
    this->flash = flash;
}

inode_v_map Ifile::get_inode_v_map() {
    inode_v_map iv_list;
    for (inode_map_itr itr = (this->i_map).begin();
         itr != (this->i_map).end(); itr++) {
        int node_id = (itr->second).get_id();
        time_t node_v = (itr->second).get_version();
        iv_list[node_id] = node_v;
    }
    return iv_list;
}

Inode * Ifile::get_inode(char * filename) {
    std::string fn (filename);
    inode_map_itr itr = (this->i_map).find(fn);
    if (itr!= (this->i_map).end()) {
        return &(itr->second);
    }
    return NULL;
}

bool Ifile::add_inode(Inode * inode) {
    std::string fn (inode->get_fn());
    (this->i_map)[fn] = *inode;
    return true;
}
