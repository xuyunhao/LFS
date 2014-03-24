#include "flash/logging_flash.h"
#include "util/logging.h"
namespace lfs {

LoggingFlash::LoggingFlash(Flash* inner_flash) {
  assert(inner_flash != NULL);
  this->inner_flash_ = inner_flash;
  this->set_nsector(inner_flash->nsector());
  this->set_noctet_sector(inner_flash->noctet_sector());
}

bool LoggingFlash::Seek(FlashSectorNum sector) {
  bool res = this->inner_flash()->Seek(sector);
  Logging::Log(kLLDebug, kLCFlash, "Seek(%"PRIu32") %d", sector, res);
  return res;
}

bool LoggingFlash::Read(FlashSectorNum sector_count, uint8_t* buf) {
  bool res = this->inner_flash()->Read(sector_count, buf);
  Logging::Log(kLLDebug, kLCFlash, "Read(%"PRIu32") %d", sector_count, res);
  return res;
}

bool LoggingFlash::Write(FlashSectorNum sector_count, const uint8_t* buf) {
  bool res = this->inner_flash()->Write(sector_count, buf);
  Logging::Log(kLLDebug, kLCFlash, "Read(%"PRIu32") %d", sector_count, res);
  return res;
}

};//namespace lfs
