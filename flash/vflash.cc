#include "flash/vflash.h"
#include <sys/types.h>
#include "util/logging.h"
namespace lfs {

VFlash::VFlash(void) {
  this->open_ = false;
  this->inner_flash_ = NULL;
  this->cur_sector_ = 0;
}

bool VFlash::Open(const char* filename) {
  assert(filename != NULL);
  this->Close();
  u_int sector_count;
  ::Flash_Create((char*)filename, 1024, 100000);
  this->inner_flash_ = ::Flash_Open((char*)filename, 0, &sector_count);
  if (this->inner_flash_ == NULL) return false;
  this->set_nsector(sector_count);
  this->set_noctet_sector(FLASH_SECTOR_SIZE);
  this->open_ = true;
  return true;
}

bool VFlash::Close(void) {
  if (!this->open_) return false;
  this->open_ = false;
  int res = ::Flash_Close(this->inner_flash_);
  return res == 0;
}

bool VFlash::Seek(FlashSectorNum sector) {
  assert(this->open_);
  if (sector >= this->nsector()) {
    Logging::Log(kLLWarn, kLCFlash, "VFlash::Seek(%"PRIu32") seek beyond end", sector);
    return false;
  }
  this->cur_sector_ = sector;
  return true;
}

bool VFlash::Read(FlashSectorNum sector_count, uint8_t* buf) {
  assert(this->open_);
  int res = ::Flash_Read(this->inner_flash_, (u_int)this->cur_sector_, (u_int)sector_count, buf);
  if (res != 0) {
    Logging::Log(kLLError, kLCFlash, "VFlash::Read(sector=%"PRIu32",count=%"PRIu32") Flash_Read error=%d", this->cur_sector_, sector_count, res);
  }
  return res == 0;
}

bool VFlash::Write(FlashSectorNum sector_count, const uint8_t* buf) {
  assert(this->open_);
  int res = ::Flash_Write(this->inner_flash_, (u_int)this->cur_sector_, (u_int)sector_count, (void*)buf);
  if (res != 0) {
    Logging::Log(kLLError, kLCFlash, "VFlash::Write(sector=%"PRIu32",count=%"PRIu32") Flash_Write error=%d", this->cur_sector_, sector_count, res);
  }
  return res == 0;
}

};//namespace lfs
