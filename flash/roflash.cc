#include "flash/roflash.h"
namespace lfs {

ReadOnlyFlash::ReadOnlyFlash(Flash* inner_flash) {
  assert(inner_flash != NULL);
  this->inner_flash_ = inner_flash;
  this->set_nsector(inner_flash->nsector());
  this->set_noctet_sector(inner_flash->noctet_sector());
}

bool ReadOnlyFlash::Seek(FlashSectorNum sector) {
  bool res = this->inner_flash()->Seek(sector);
  return res;
}

bool ReadOnlyFlash::Read(FlashSectorNum sector_count, uint8_t* buf) {
  bool res = this->inner_flash()->Read(sector_count, buf);
  return res;
}

bool ReadOnlyFlash::Write(FlashSectorNum sector_count, const uint8_t* buf) {
  return false;
}

};//namespace lfs
