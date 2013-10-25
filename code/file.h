//
//  file.h
//  
//
//  Created by Dennis Xu on 10/24/13.
//
//

#ifndef ____file__
#define ____file__

#include <iostream>
#include <string>
#include <cstring>
#include "lld.h"
#include "flash.h"
#include "inode.h"
#include "log.h"

class File {
public:
    File(Inode * inode, Log* log);
    ~File();
    
    Inode * get_inode();
    Log * get_log();
    
    bool File_Read(int off, int len, char * buf);
    bool File_Write(int len, char * buf);
    
private:
    Inode * node;
    Log * lg;
    
    char * content;
};

inline Inode * File::get_inode() { return this->node;}
inline Log * File::get_log() {return this->lg;}
#endif /* defined(____file__) */
