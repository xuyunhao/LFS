//
//  ifile.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____ifile__
#define ____ifile__

#include <string>
#include <cstring>
#include <map>
#include "inode.h"
#include "lld.h"
#include "log.h"

typedef std::map<std::string, Inode*> inode_map;
typedef std::map<std::string, Inode*>::iterator inode_map_itr;

typedef std::map<int, time_t> inode_v_map;
typedef std::map<int, time_t>::iterator inode_v_map_itr;

class Ifile {
public:
    Ifile(Flash flash, logAddress * la);
    ~Ifile();
    
    inode_map get_inode_map();
    inode_v_map get_inode_v_map();
    
    Inode * get_inode(char * filename);
    
    bool add_inode(Inode * inode);
    
    bool write_to_flash(Log * log);
    
    logAddress * get_log_addr();
private:
    Flash flash;
    inode_map i_map;
    logAddress * la;
};

inline inode_map Ifile::get_inode_map() { return this->i_map;}
inline logAddress * Ifile::get_log_addr() {return this->la;}

#endif /* defined(____ifile__) */
