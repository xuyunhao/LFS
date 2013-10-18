//
//  mklfs.cc
//  
//
//  Created by Dennis Xu on 10/18/13.
//
//

#include "mklfs.h"
#include "flash.h"
#include <getopt.h>


bool mklfs_getop(int argc, char** argv, struct mklfs_opts* opts){
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"segment", required_argument, 0, 0},
            {"sectors", required_argument, 0, 0},
            {"block", required_argument, 0, 0},
            {"wearlimit", required_argument, 0, 0}
        };
        int c = ::getopt_long(argc, argv, "l:s:b:w:", long_options, &option_index);
        if (c == -1) break;
        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0: opts->segment_num = ::atoll(optarg); break;
                    case 1: opts->sector_num = ::atol(optarg); break;
                    case 2: opts->blk_num = ::atol(optarg); break;
                    case 3: opts->wearlimit = ::atol(optarg); break;
                }
            } break;
            case 'l': opts->segment_num = ::atoll(optarg); break;
            case 's': opts->sector_num = ::atol(optarg); break;
            case 'b': opts->blk_num = ::atol(optarg); break;
            case 'w': opts->wearlimit = ::atol(optarg); break;
        }
    }
    if (optind < argc) {
        opts->filename = argv[optind];
    }
    if (opts->segment_num == 0) opts->segment_num = 32768;
    if (opts->sector_num == 0) opts->sector_num = 1024;
    if (opts->blk_num == 0) opts->blk_num = 2;
    if (opts->wearlimit == 0) opts->wearlimit = 1000;
    return opts->filename != NULL;
}

int main(int argc, char** argv) {
    struct mklfs_opts opts = {0};
    if (! mklfs_getop (argc, argv, & opts)) {
        // TODO
        // error message
        std::cout << "Usage: mklfs [OPTION] file\n";
        std::cout << "\t-l size, --segment=size\n";
        std::cout << "\t\t Segment size, in blocks. The default is 32.\n";
        std::cout << "\t-s sectors, --sectors=sectors\n";
        std::cout << "\t\t Size of the flash, in sectors.  The default is 1024.\n";
        std::cout << "\t-b size, --block=size\n";
        std::cout << "\t\t Size of a block, in sectors. The default is 2 (1KB).\n";
        std::cout << "\t-w limit, --wearlimit=limit\n";
        std::cout << "\t\t Wear limit for erase blocks. The default is 1000.\n"<<std::endl;
    }
    
    // TODO
    // Create log
    
    // Create flash
    Flash_Create(opts.filename, opts.wearlimit, opts.blk_num);
    
}