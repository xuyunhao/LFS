#ifndef LFS_FLASH_FLASH_H_
#define LFS_FLASH_FLASH_H_
#include "util/defs.h"
namespace lfs {

typedef uint32_t FlashSectorNum;

//abstract Flash class
class Flash {
  public:
    FlashSectorNum nsector(void) const { return this->nsector_; }
    uint32_t noctet_sector(void) const { return this->noctet_sector_; }
    virtual bool Seek(FlashSectorNum sector) =0;
    virtual bool Read(FlashSectorNum sector_count, uint8_t* buf) =0;
    virtual bool Write(FlashSectorNum sector_count, const uint8_t* buf) =0;
    
  protected:
    Flash(void);
    void set_nsector(FlashSectorNum value) { this->nsector_ = value; }
    void set_noctet_sector(uint32_t value) { this->noctet_sector_ = value; }
    
  private:
    FlashSectorNum nsector_;
    uint16_t noctet_sector_;
    DISALLOW_COPY_AND_ASSIGN(Flash);
};

inline Flash::Flash(void) {
  this->nsector_ = 0;
  this->noctet_sector_ = 0;
}

};//namespace lfs
#endif//LFS_FLASH_FLASH_H_
