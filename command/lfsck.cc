#include "flash/vflash.h"
#include "fs/verifier.h"
namespace lfs {

struct lfsck_cmdopts {
  char* filename;
};

bool lfsck_getopt(int argc, char** argv, struct lfsck_cmdopts* opts) {
  opts->filename = argv[1];
  return opts->filename != NULL;
}

int lfsck_main(int argc, char** argv) {
  struct lfsck_cmdopts opts = {0};
  if (!lfsck_getopt(argc, argv, &opts)) {
    ::printf("lfs, a command to verify an LFS flash.\n"
             "Usage: lfsck filename\n");
    return 1;
  }
  int res = 0;

  VFlash flash; flash.Open(opts.filename);
  
  FsVerifier verifier;
  if (!verifier.Verify(&flash)) {
    res = 1;
    const std::vector<std::string>* msgs = verifier.msgs();
    for (std::vector<std::string>::const_iterator it = msgs->begin(); it < msgs->end(); ++it) {
      ::printf("%s\n", it->c_str());
    }
  }
  
  flash.Close();
  
  return res;
}

};//namespace lfs
