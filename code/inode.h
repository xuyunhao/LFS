//
//  inode.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____inode__
#define ____inode__

#include <cstring>
#include <map>
#include <vector>
#include <ctime>

#include "lld.h"

typedef std::map<int, logAddress*> addr_map;
typedef std::map<int, logAddress*>::iterator addr_map_itr;
typedef std::vector<logAddress*> addr_list;

class Inode{
public:
    Inode();
    Inode(Flash flash, int node_id, char fileaname[]);
    ~Inode();
    
    int get_id();
    char * get_fn();
    time_t get_version();
    addr_list get_blk_list();
    bool update(int offset, logAddress * addr);
    char * to_string();
private:
    Flash flash;
    int node_id;
    char filename[MAX_FILE_NAME_SIZE];
    addr_map blk_map;
    time_t version;
};

inline int Inode::get_id() {return this->node_id;}
inline char * Inode::get_fn() {return this->filename;}
inline time_t Inode::get_version() {return this->version;}


#endif /* defined(____inode__) */
