#include "flash/memflash.h"
#include <cstdlib>
#include <cstring>
namespace lfs {

MemFlash::MemFlash(void) {
  this->buf_ = NULL;
  this->cur_sector_ = 0;
}

MemFlash::~MemFlash(void) {
  if (this->buf_ != NULL) free(this->buf_);
}

bool MemFlash::Init(uint32_t noctet_sector, FlashSectorNum nsector) {
  assert(this->buf_ == NULL);
  assert(noctet_sector > 0);
  this->set_noctet_sector(noctet_sector);
  this->set_nsector(nsector);
  this->buf_ = (uint8_t*)std::calloc(nsector, noctet_sector);
  if (this->buf_ == NULL) return false;
  return true;
}

bool MemFlash::Seek(FlashSectorNum sector) {
  if (sector < 0 || sector >= this->nsector()) return false;
  this->cur_sector_ = sector;
  return true;
}

void MemFlash::CalcOffset(FlashSectorNum sector_count, uint8_t** offset, size_t* count) const {
  *offset = this->buf_ + this->cur_sector_ * this->noctet_sector();
  *count = sector_count * this->noctet_sector();
}

bool MemFlash::Read(FlashSectorNum sector_count, uint8_t* buf) {
  assert(this->buf_ != NULL);
  uint8_t* offset; size_t count;
  this->CalcOffset(sector_count, &offset, &count);
  std::memcpy(buf, offset, count);
  return true;
}

bool MemFlash::Write(FlashSectorNum sector_count, const uint8_t* buf) {
  assert(this->buf_ != NULL);
  uint8_t* offset; size_t count;
  this->CalcOffset(sector_count, &offset, &count);
  std::memcpy(offset, buf, count);
  return true;
}

};//namespace lfs
