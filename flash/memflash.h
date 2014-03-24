#ifndef LFS_FLASH_MEMFLASH_H_
#define LFS_FLASH_MEMFLASH_H_
#include "util/defs.h"
#include "flash/flash.h"
namespace lfs {

//a Flash backed by memory
class MemFlash : public Flash {
  public:
    MemFlash(void);
    ~MemFlash(void);
    bool Init(uint32_t noctet_sector, FlashSectorNum nsector);
    virtual bool Seek(FlashSectorNum sector);
    virtual bool Read(FlashSectorNum sector_count, uint8_t* buf);
    virtual bool Write(FlashSectorNum sector_count, const uint8_t* buf);
    
  private:
    void CalcOffset(FlashSectorNum sector_count, uint8_t** offset, size_t* count) const;
    uint8_t* buf_;
    FlashSectorNum cur_sector_;
    DISALLOW_COPY_AND_ASSIGN(MemFlash);
};

};//namespace lfs
#endif//LFS_FLASH_VFLASH_H_
