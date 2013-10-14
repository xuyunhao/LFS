//
//  ifile.h
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#ifndef ____ifile__
#define ____ifile__

#include <"lld.h">
#include <"inode.h">

struct ifile {
    std::list<struct inode> inode_list;
}

#endif /* defined(____ifile__) */
