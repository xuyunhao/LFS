//
//  lfsck.cc
//  
//
//  Created by Dennis Xu on 10/24/13.
//
//

#include "lfsck.h"
#include "flash.h"
#include <getopt.h>
#include "superblock.h"
#include "checkpoint.h"

bool lfsck_getopt(int argc, char** argv, struct lfsck_cmdopts* opts) {
    opts->filename = argv[1];
    return opts->filename != NULL;
}

int main(int argc, char** argv) {
    struct lfsck_cmdopts opts = {0};
    if (! lfsck_getopt (argc, argv, & opts)) {
        // TODO
        // error message
        std::cout << "Usage: lfsck filename\n"<<std::endl;
    }
    
    // Create flash
    //give disk file name,
    //erase block wear limit,
    //and how many erase blocks the flash should have--which is total sectors/sectors per block
    Flash_Create(opts.filename,1000, 1024/2);
    u_int size;
    Flash flash = Flash_Open(opts.filename, FLASH_SILENT, &size);
    // TODO
    // Create log
    Log log;
    log.open(flash);
    SuperBlock sb(flash, &log);
    
    CheckPoint * cp = sb.get_most_recent_checkpoint();
    Flash_Close(flash);

}
