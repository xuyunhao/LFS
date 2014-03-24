#ifndef LFS_FLASH_ROFLASH_H_
#define LFS_FLASH_ROFLASH_H_
#include "util/defs.h"
#include "flash/flash.h"
namespace lfs {

//a Flash that is read-only
class ReadOnlyFlash : public Flash {
  public:
    explicit ReadOnlyFlash(Flash* inner_flash);
    Flash* inner_flash(void) const { return this->inner_flash_; }
    virtual bool Seek(FlashSectorNum sector);
    virtual bool Read(FlashSectorNum sector_count, uint8_t* buf);
    virtual bool Write(FlashSectorNum sector_count, const uint8_t* buf);
    
  private:
    Flash* inner_flash_;
    DISALLOW_COPY_AND_ASSIGN(ReadOnlyFlash);
};

};//namespace lfs
#endif//LFS_FLASH_ROFLASH_H_
