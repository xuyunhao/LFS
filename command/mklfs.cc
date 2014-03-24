#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "flash/vflash.h"
#include "log/logcreator.h"
#include "file/sysfile_creator.h"
namespace lfs {

struct mklfs_cmdopts {
  size_t noctet_seg;
  FlashSectorNum nsector_flash;
  FlashSectorNum nsector_blk;
  char* filename;
};

bool mklfs_getopt(int argc, char** argv, struct mklfs_cmdopts* opts) {
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"segment", required_argument, 0, 0},
      {"sectors", required_argument, 0, 0},
      {"block", required_argument, 0, 0}
    };
    int c = ::getopt_long(argc, argv, "l:s:b:", long_options, &option_index);
    if (c == -1) break;
    switch (c) {
      case 0: {
        switch (option_index) {
          case 0: opts->noctet_seg = ::atoll(optarg); break;
          case 1: opts->nsector_flash = ::atol(optarg); break;
          case 2: opts->nsector_blk = ::atol(optarg); break;
        }
      } break;
      case 'l': opts->noctet_seg = ::atoll(optarg); break;
      case 's': opts->nsector_flash = ::atol(optarg); break;
      case 'b': opts->nsector_blk = ::atol(optarg); break;
    }
  }
  if (optind < argc) {
    opts->filename = argv[optind];
  }
  if (opts->noctet_seg == 0) opts->noctet_seg = 32768;
  if (opts->nsector_flash == 0) opts->nsector_flash = 1024;
  if (opts->nsector_blk == 0) opts->nsector_blk = 2;
  return opts->filename != NULL;
}

int mklfs_main(int argc, char** argv) {
  struct mklfs_cmdopts opts = {0};
  if (!mklfs_getopt(argc, argv, &opts)) {
    ::printf("mklfs, a command to create and format a flash for LFS.\n"
             "Usage: mklfs [OPTION]... filename\n"
             "\n"
             "  -l, --segment=noctet_seg\n"
             "  -s, --sectors=nsector_flash\n"
             "  -b, --block=nsector_blk\n");
    return 1;
  }

  LogCreator lc;
  lc.set_noctet_sector(FLASH_SECTOR_SIZE);
  lc.set_nsector_blk(opts.nsector_blk);
  lc.set_nblk_seg(opts.noctet_seg / FLASH_SECTOR_SIZE / opts.nsector_blk);
  lc.set_nseg_flash(opts.nsector_flash * FLASH_SECTOR_SIZE / opts.noctet_seg);
  if (!lc.VerifyMetrics()) {
    ::printf("LogCreator::VerifyMetrics fails\n");
    return 1;
  }

  int fd = ::creat(opts.filename, S_IRWXU);
  if (fd == 0) {
    ::printf("creat fails\n");
    return 1;
  }
  ::ftruncate(fd, lc.noctet_flash());
  ::close(fd);

  VFlash flash;
  flash.Open(opts.filename);
  if (!lc.set_flash(&flash)) {
    ::printf("LogCreator::set_flash fails\n");
    return 1;
  }
  if (!lc.WriteSuperblock()) {
    ::printf("LogCreator::WriteSuperblock fails\n");
    return 1;
  }
  if (!lc.WriteCheckpoints()) {
    ::printf("LogCreator::WriteCheckpoints fails\n");
    return 1;
  }
  
  Log log; log.Open(&flash);
  InodeStore istore; istore.Open(&log);
  SystemFileCreator sfc(&istore);
  if (!sfc.CreateAll()) {
    ::printf("SystemFileCreator::CreateAll fails\n");
    return 1;
  }
  istore.Close();
  log.Close();
  
  flash.Close();
  return 0;
}

};//namespace lfs
