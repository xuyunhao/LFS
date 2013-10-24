//
//  checkpoint.h
//  
//
//  Created by Dennis Xu on 10/22/13.
//
//

#ifndef ____checkpoint__
#define ____checkpoint__

#include <iostream>
#include <ctime>
#include <vector>
#include "log.h"
#include "lld.h"

class CheckPoint {
public:
    //CheckPoint(Log * log, Ifile * ifile);
    CheckPoint(Log * log);
    ~CheckPoint();
    
    int     get_tail_seg_id();
    int     get_tail_blk_id();
    
    time_t  get_version();
    
    int     get_ifile_inode_seg_id();
    int     get_ifile_inode_blk_id();
    
    char *  to_string();
private:
    time_t  version;
    int     tail_seg_id;
    int     tail_blk_id;

    int     ifile_inode_seg_id;
    int     ifile_inode_blk_id;

    
    // TODO
//    std::map<int, time_t> inode_version_table;
//    std::map<int, bool> seg_usage_table;
};

inline int CheckPoint::get_tail_seg_id() { return this->tail_seg_id;}
inline int CheckPoint::get_tail_blk_id() { return this->tail_blk_id;}
inline time_t CheckPoint::get_version() { return this->version;}
inline int CheckPoint::get_ifile_inode_seg_id() { return this->ifile_inode_seg_id;}
inline int CheckPoint::get_ifile_inode_blk_id() { return this->ifile_inode_blk_id;}

#endif /* defined(____checkpoint__) */
