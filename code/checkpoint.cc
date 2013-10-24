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

CheckPoint::~CheckPoint() {}

char *  CheckPoint::to_string() {
    char * str = (char *) malloc(sizeof(checkpoint));
    checkpoint * cp = (checkpoint *) str;
    cp->version = this->version;
    cp->tail_seg_id = this->tail_seg_id;
    cp->tail_blk_id = this->tail_blk_id;
    
    cp->ifile_inode_seg_id = this->ifile_inode_seg_id;
    cp->ifile_inode_blk_id = this->ifile_inode_blk_id;

//    inode_version_table * ivt = (inode_version_table *) str + sizeof(checkpoint);
//    typedef std::map<int, time_t>::iterator ivt_type;
//    for(ivt_type iterator = this->inode_version_table.begin();
//        iterator != this->inode_version_table.end();
//        iterator++)
//    {
//        ivt->inode_id = iterator->inode_id;
//        ivt->inode_v = iterator->inode_v;
//        ivt += sizeof(inode_version_table);
//    }
//    seg_usage_table * sut = (seg_usage_table *) ivt;
//    typedef std::map<int, bool>::iterator sut_type;
//    for(sut_type iterator = this->seg_usage_table.begin();
//        iterator != this->inode_version_table.end();
//        iterator++)
//    {
//        sut->seg_id = iterator->seg_id;
//        if (iterator->seg_used) {
//            sut->is_used = 1;
//        }
//        else {
//            sut->is_used = 0;
//        }
//        sut += sizeof(seg_usage_table);
//    }
    
    return str;
}