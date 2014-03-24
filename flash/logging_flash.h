#ifndef LFS_FLASH_LOGGING_FLASH_H_
#define LFS_FLASH_LOGGING_FLASH_H_
#include "util/defs.h"
#include "flash/flash.h"
namespace lfs {

//a Flash that writes every operation to the Logging component
class LoggingFlash : public Flash {
  public:
    explicit LoggingFlash(Flash* inner_flash);
    Flash* inner_flash(void) const { return this->inner_flash_; }
    virtual bool Seek(FlashSectorNum sector);
    virtual bool Read(FlashSectorNum sector_count, uint8_t* buf);
    virtual bool Write(FlashSectorNum sector_count, const uint8_t* buf);
    
  private:
    Flash* inner_flash_;
    DISALLOW_COPY_AND_ASSIGN(LoggingFlash);
};

};//namespace lfs
#endif//LFS_FLASH_LOGGING_FLASH_H_
