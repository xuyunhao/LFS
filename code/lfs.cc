//
//  lfs.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "lfs.h"
#include "flash.h"
#include <getopt.h>
#include "superblock.h"
#include "checkpoint.h"

bool lfs_getop(int argc, char** argv, struct lfs_cmdopts* opts){
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"cache", required_argument, 0, 0},
            {"interval", required_argument, 0, 0},
            {"start", required_argument, 0, 0},
            {"stop", required_argument, 0, 0}
        };
        int c = ::getopt_long(argc, argv, "s:i:c:C:", long_options, &option_index);
        if (c == -1) break;
        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0: opts->nseg_cache = ::atol(optarg); break;
                    case 1: opts->checkpoint_interval = ::atol(optarg); break;
                    case 2: opts->cleaning_start = ::atol(optarg); break;
                    case 3: opts->cleaning_stop = ::atol(optarg); break;
                }
            } break;
            case 's': opts->nseg_cache = ::atol(optarg); break;
            case 'i': opts->checkpoint_interval = ::atol(optarg); break;
            case 'c': opts->cleaning_start = ::atol(optarg); break;
            case 'C': opts->cleaning_stop = ::atol(optarg); break;
        }
    }
    if (optind + 1 < argc) {
        opts->filename = argv[optind];
        opts->mountpoint = argv[optind + 1];
    }
    if (opts->nseg_cache == SEGNUM_INVALID) opts->nseg_cache = 16;
    if (opts->checkpoint_interval == 0) opts->checkpoint_interval = 4;
    if (opts->cleaning_start == 0) opts->cleaning_start = 4;
    if (opts->cleaning_stop == 0) opts->cleaning_stop = 8;
    return opts->filename != NULL && opts->mountpoint != NULL;
}

int main(int argc, char** argv) {
    struct lfs_cmdopts opts = {0};
    if (! lfs_getop (argc, argv, & opts)) {
        // TODO
        // error message
        std::cout << "Usage: lfs [options] file mountpoint\n";
        std::cout << "\ts num, --cache=num\n";
        std::cout << "\t\t Size of the cache in the Log layer, in segments.\n";
        std::cout << "\t-i num, --interval=num\n";
        std::cout << "\t\t Checkpoint interval, in segments.\n";
        std::cout << "\t-c num, --start=num\n";
        std::cout << "\t\t Threshold at which cleaning starts, in segments. Default is 4.\n";
        std::cout << "\t-C num, --stop=num\n";
        std::cout << "\t\t Threshold at which cleaning stops, in segments. Default is 8.\n"<<std::endl;
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
