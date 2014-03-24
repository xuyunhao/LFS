#ifndef LFS_FLASH_VFLASH_H_
#define LFS_FLASH_VFLASH_H_
#include "util/defs.h"
#include "exttool/flash.h"
#include "flash/flash.h"
namespace lfs {

//a flash backed by CSC552 flash.h
class VFlash : public Flash {
  public:
    VFlash();
    bool Open(const char* filename);
    bool Close(void);
    virtual bool Seek(FlashSectorNum sector);
    virtual bool Read(FlashSectorNum sector_count, uint8_t* buf);
    virtual bool Write(FlashSectorNum sector_count, const uint8_t* buf);
    
  private:
    bool open_;//true after successful Init(), false after Close()
    ::Flash inner_flash_;
    FlashSectorNum cur_sector_;
    DISALLOW_COPY_AND_ASSIGN(VFlash);
};

};//namespace lfs
#endif//LFS_FLASH_VFLASH_H_
