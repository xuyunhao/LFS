#include "log/logcreator.h"
#include <cstdlib>
#include <cstring>
#include "log/checkpoint.h"
namespace lfs {

LogCreator::LogCreator(void) {
  this->sbc_ = new SuperblockCreator();
}

LogCreator::~LogCreator(void) {
  delete this->sbc_;
}

bool LogCreator::VerifyMetrics(void) const {
  return (
    SegNum_nseg_min <= this->nseg_flash() && this->nseg_flash() <= SegNum_nseg_max &&
    BlkNum_nblk_min <= this->nblk_seg() && this->nblk_seg() <= BlkNum_nblk_max &&
    this->nsector_blk() <= 1024 &&
    this->noctet_sector() * this->nsector_blk() >= 512
  );
}

size_t LogCreator::noctet_flash(void) const {
  return this->nseg_flash() * this->nblk_seg() * this->nsector_blk() * this->noctet_sector();
}

bool LogCreator::set_flash(Flash* flash) {
  if (!this->VerifyMetrics() || !(
    this->noctet_sector() == flash->noctet_sector() &&
    this->nseg_flash() * this->nblk_seg() * this->nsector_blk() <= flash->nsector()
  )) return false;
  this->flash_ = flash;
  return true;
}

void LogCreator::SuperblockCreator::set_flash(Flash* flash) {
  this->flash_ = flash;
  this->set_noctet_sector(flash->noctet_sector());
}

void LogCreator::SuperblockCreator::SetMetrics(FlashSectorNum nsector_blk, BlkNum nblk_seg, SegNum nseg_flash) {
  this->set_nsector_blk(nsector_blk);
  this->set_nblk_seg(nblk_seg);
  this->set_nseg_flash(nseg_flash);
  
  size_t noctet_checkpoint = 4 + 461 + 14 * this->nseg_flash();
  size_t nblk_checkpoint = noctet_checkpoint / this->noctet_blk()
                           + (noctet_checkpoint % this->noctet_blk() == 0 ? 0 : 1);
  SegNum nseg_checkpoint = (nblk_checkpoint + 1) / this->nblk_seg()
                           + ((nblk_checkpoint + 1) % this->nblk_seg() == 0 ? 0 : 1);
  this->set_checkpoint_seg(0, 1);
  this->set_checkpoint_seg(1, 1 + nseg_checkpoint);
  this->set_first_logseg(1 + nseg_checkpoint + nseg_checkpoint);
  this->set_nseg_checkpoint(nseg_checkpoint);

  size_t noctet_segsummary = 24 + 24 * this->nblk_seg();
  BlkNum nblk_segsummary = noctet_segsummary / this->noctet_blk()
                           + (noctet_segsummary % this->noctet_blk() == 0 ? 0 : 1);
  this->set_nblk_segsummary(nblk_segsummary);
}

bool LogCreator::SuperblockCreator::WriteToFlash(void) const {
  FlashSectorNum sector = 0;
  uint8_t* buf = (uint8_t*)::malloc(this->noctet_blk());

  sector += this->nsector_blk();//seg 0 blk 1 - magic
  memcpy(buf, this->kMagic, sizeof(this->kMagic));
  this->flash()->Seek(sector);
  this->flash()->Write(this->nsector_blk(), buf);
  
  sector += this->nsector_blk();//seg 0 blk 2 - protobuf
  this->Write(buf);
  this->flash()->Seek(sector);
  this->flash()->Write(this->nsector_blk(), buf);
  
  ::free(buf);
  
  return true;
}


bool LogCreator::WriteSuperblock(void) const {
  this->sbc_->set_flash(this->flash());
  this->sbc_->SetMetrics(this->nsector_blk(), this->nblk_seg(), this->nseg_flash());
  return this->sbc_->WriteToFlash();
}

bool LogCreator::WriteCheckpoints(void) const {
  SegIO segio(this->flash(), this->superblock(), 0);
  Checkpoint cp(&segio);
  cp.set_timestamp(DateTime_now());
  cp.set_next_seg(this->superblock()->first_logseg());
  Inode inode;
  inode.set_ino(Inode::kIno_InoMap); cp.set_inode_inomap(&inode);
  inode.set_ino(Inode::kIno_InoVec); cp.set_inode_inovec(&inode);
  inode.set_ino(Inode::kIno_InoTbl); cp.set_inode_inotbl(&inode);
  return cp.Write(0);
}


};//namespace lfs
