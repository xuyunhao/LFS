//
//  ifile.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "ifile.h"

Ifile::Ifile(Flash flash, logAddress * la){
    this->flash = flash;
    this->la = la;
}

inode_v_map Ifile::get_inode_v_map() {
    inode_v_map iv_list;
    for (inode_map_itr itr = (this->i_map).begin();
         itr != (this->i_map).end(); itr++) {
        int node_id = (itr->second)->get_id();
        time_t node_v = (itr->second)->get_version();
        iv_list[node_id] = node_v;
    }
    return iv_list;
}

Inode * Ifile::get_inode(char * filename) {
    std::string fn (filename);
    inode_map_itr itr = (this->i_map).find(fn);
    if (itr!= (this->i_map).end()) {
        return itr->second;
    }
    return NULL;
}

bool Ifile::add_inode(Inode * inode) {
    std::string fn (inode->get_fn());
    (this->i_map)[fn] = inode;
    return true;
}

bool Ifile::write_to_flash(Log * log) {
    int in_size = MAX_BLOCK_PER_FILE * sizeof(direct_block)+sizeof(inode);
    int size = (this->i_map).size() * in_size;
    char * str = (char *) malloc(size);
    char * inode_str = str;
    for (inode_map_itr itr = (this->i_map).begin(); itr != (this->i_map).end(); itr++) {
        char * in_str = (itr->second)->to_string();
        memcpy(inode_str, in_str, in_size);
        inode_str += in_size;
    }
    return log->Log_Ifile_Write(size, str, *(this->la));
}