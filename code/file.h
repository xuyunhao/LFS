//
//  file.h
//  
//
//  Created by Dennis Xu on 10/20/13.
//
//

#ifndef ____file__
#define ____file__

#include <iostream>
//TODO
class file {
public:
    file();
    ~file();
    inode* get_inode() const {return this->inode};
    log* get_log() const { return this->inode};
    
    bool Read();
    bool write();
    
private:
    inode *inode;
    log *log;
}

#endif /* defined(____file__) */
