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
#include "ifile.h"

typedef std::map<int, time_t> inode_v_map;
typedef std::map<int, time_t>::iterator inode_v_map_itr;

typedef std::map<int, bool> seg_u_map;
typedef std::map<int, bool>::iterator seg_u_map_itr;

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
    inode_v_map inode_vt;
    seg_u_map seg_ut;
};

inline int CheckPoint::get_tail_seg_id() { return this->tail_seg_id;}
inline int CheckPoint::get_tail_blk_id() { return this->tail_blk_id;}
inline time_t CheckPoint::get_version() { return this->version;}
inline int CheckPoint::get_ifile_inode_seg_id() { return this->ifile_inode_seg_id;}
inline int CheckPoint::get_ifile_inode_blk_id() { return this->ifile_inode_blk_id;}

#endif /* defined(____checkpoint__) */
