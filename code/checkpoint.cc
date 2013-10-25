//
//  checkpoint.cc
//  
//
//  Created by Dennis Xu on 10/22/13.
//
//

#include "checkpoint.h"
//#include <cstdlib>

//CheckPoint::CheckPoint(Flash * flash, Log * log, Ifile * ifile) {
CheckPoint::CheckPoint(Log * log) {
    time(&(this->version));
    this->tail_seg_id = (log->getLogAddress())->seg_num;
    this->tail_blk_id = (log->getLogAddress())->blk_num;
    
    //TODO
//    this->ifile_inode_seg_id;
//    this->ifile_inode_blk_id;
    // TODO
//    map<int, time_t> inode_version_table;
//    map<int, bool> seg_usage_table;


}

CheckPoint::~CheckPoint() {
    
}

char *  CheckPoint::to_string() {
    char * str = (char *) malloc(sizeof(checkpoint));
    checkpoint * cp = (checkpoint *) str;
    cp->version = this->version;
    cp->tail_seg_id = this->tail_seg_id;
    cp->tail_blk_id = this->tail_blk_id;
    
    cp->ifile_inode_seg_id = this->ifile_inode_seg_id;
    cp->ifile_inode_blk_id = this->ifile_inode_blk_id;

    inode_version_table * ivt = (inode_version_table *) str + sizeof(checkpoint);
    for(inode_v_map_itr iterator = this->inode_vt.begin();
        iterator != this->inode_vt.end();
        iterator++)
    {
        ivt->inode_id = iterator->first;
        ivt->inode_v = iterator->second;
        ivt += sizeof(inode_version_table);
    }
    seg_usage_table * sut = (seg_usage_table *) ivt;
    for(seg_u_map_itr iterator = this->seg_ut.begin();
        iterator != this->seg_ut.end();
        iterator++)
    {
        sut->seg_id = iterator->first;
        if (iterator->second) {
            sut->is_used = 1;
        }
        else {
            sut->is_used = 0;
        }
        sut += sizeof(seg_usage_table);
    }
    
    return str;
}