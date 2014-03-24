/*
procedure of creating a log
1. lc.set_<metrics>
2. lc.VerifyMetrics()
3. create a flash of length lc.noctets_flash()
4. lc.set_flash()
5. lc.WriteSuperblock()
6. lc.WriteCheckpoints()

*/
#ifndef LFS_LOG_LOGCREATOR_H_
#define LFS_LOG_LOGCREATOR_H_
#include "util/defs.h"
#include "flash/flash.h"
#include "log/superblock.h"
namespace lfs {

class LogCreator {
  public:
    LogCreator(void);
    ~LogCreator(void);
    size_t noctet_sector(void) const { return this->noctet_sector_; }
    void set_noctet_sector(size_t value) { this->noctet_sector_ = value; }
    FlashSectorNum nsector_blk(void) const { return this->nsector_blk_; }
    void set_nsector_blk(FlashSectorNum value) { this->nsector_blk_ = value; }
    BlkNum nblk_seg(void) const { return this->nblk_seg_; }
    void set_nblk_seg(BlkNum value) { this->nblk_seg_ = value; }
    SegNum nseg_flash(void) const { return this->nseg_flash_; }
    void set_nseg_flash(SegNum value) { this->nseg_flash_ = value; }

    size_t noctet_flash(void) const;
    Flash* flash(void) const { return this->flash_; }
    bool set_flash(Flash* flash);//set flash, returns true if flash meets metrics

    bool VerifyMetrics(void) const;//verify metrics meet constraints
    bool WriteSuperblock(void) const;//write superblock
    bool WriteCheckpoints(void) const;//write empty checkpoints (inodes have zero length)

  private:
    class SuperblockCreator : public Superblock {
      public:
        SuperblockCreator(void) {}
        Flash* flash(void) const { return this->flash_; }
        void set_flash(Flash* flash);//set flash, returns true if flash meets metrics
        void SetMetrics(FlashSectorNum nsector_blk, BlkNum nblk_seg, SegNum nseg_flash);
        bool WriteToFlash(void) const;
      private:
        Flash* flash_;
    };
    
    size_t noctet_sector_;//count of octets per sector
    FlashSectorNum nsector_blk_;//count of sectors per block
    BlkNum nblk_seg_;//count of blocks per segment
    SegNum nseg_flash_;//count of segments on the flash
    Flash* flash_;
    SuperblockCreator* sbc_;
    Superblock* superblock(void) const { return this->sbc_; }
    DISALLOW_COPY_AND_ASSIGN(LogCreator);
};

};//namespace lfs
#endif//LFS_LOG_LOGCREATOR_H_
