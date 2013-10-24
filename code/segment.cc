//
//  segment.cc
//  
//
//  Created by Dennis Xu on 10/12/13.
//
//

#include "segment.h"
#include "block.h"

#include <ctime>
#include <string.h>
#include <vector>

#include <iostream>

void print_t(char * s) {
    segment_metadata * sm = (segment_metadata *)s;
    std::cout << "seg id: " << sm->seg_id << "\n";
    std::cout << "inode id: " << sm->inode_num << "\n";
    std::cout << "remain size: " << sm->remaining_size << "\n";
    std::cout << "version: " << sm->version << "\n";
    std::cout << "prev id: " << sm->prev_start_sector << "\n";
    std::cout << "next id: " << sm->next_start_sector << std::endl;
}
Segment::Segment(Flash flash, int start_sector_id, int pre_seg_size, int blk_per_seg, int sector_per_blk, int wearlimit) {
    this->flash = flash;
    this->sector_offset = start_sector_id;
    this->pre_seg_size = pre_seg_size;
    this->sector_per_blk = sector_per_blk;
    this->blk_per_seg = blk_per_seg;
    this->maximum_size = blk_per_seg - pre_seg_size;
    
    this->wearlimit = wearlimit;
    
    // the number of sector for a segment
    int count = blk_per_seg * sector_per_blk;
    // size in byte of the entire segment
    int seg_size_byte = count * FLASH_SECTOR_SIZE;
    // size in byte for pre-segment block
    int pre_seg_size_byte = pre_seg_size * sector_per_blk * FLASH_SECTOR_SIZE;
    
    char * seg_content = (char *)malloc(seg_size_byte);
    if (!Flash_Read(this->flash, this->sector_offset, count, seg_content)) {
        // print error message
    }
    
    this->seg_id = ((struct segment_metadata *) seg_content)->seg_id;
    this->version = ((struct segment_metadata *) seg_content)->version;
    this->remaining_size = ((struct segment_metadata *) seg_content)->remaining_size;
    this->current_usage = ((struct segment_metadata *) seg_content)->current_usage;
    
    this->prev_seg_starting_sector = ((struct segment_metadata *) seg_content)->prev_start_sector;
    this->next_seg_starting_sector = ((struct segment_metadata *) seg_content)->next_start_sector;
    
    int blk_in_used = this->blk_per_seg - this->remaining_size - this->pre_seg_size;
    this->next_free_blk_id = pre_seg_size + blk_in_used;

    print_t(seg_content);
    std::cout << this->seg_id << std::endl;

    std::cout << blk_in_used << " "<< this->blk_per_seg;
    std::cout << " " << this->remaining_size << " ";
    std::cout << this->pre_seg_size << std::endl;
    // read the block list
    // the metadata for the block list in the content
    char * blk_list_metadata = seg_content + sizeof(struct segment_metadata);
    // the content in segment (exclude the pre-segment)
    char * block_contents = seg_content + pre_seg_size_byte;
    int i = 0;
    std::cout << "111" << std::endl;
    // read data content for blocks that has been used
    for (; i < blk_in_used; i++) {
        int start = start_sector_id + (pre_seg_size+i) * sector_per_blk;
        std::cout << "333" << std::endl;
        Block * blk = new Block(this->flash, start, sector_per_blk, i+pre_seg_size, wearlimit, this->current_usage);
        std::cout << "444" << std::endl;
        blk->read_block(blk_list_metadata, block_contents, this->pre_seg_size);
        std::cout << "555" << std::endl;
        (this->blk_list).push_back(*blk);
    }
    std::cout << "222" << std::endl;
    // Initial the blocks that has not been used
    for (; i < this->maximum_size; i++) {
        int start = start_sector_id + (pre_seg_size+i) * sector_per_blk;
        std::cout << "333" << std::endl;
        Block * blk =new Block(this->flash, start, sector_per_blk, i+pre_seg_size, wearlimit, this->current_usage);
        std::cout << "444" << std::endl;
        std::cout << blk << " " << (blk == NULL)<< std::endl;
        (this->blk_list).push_back(*blk);
        std::cout << "555 "<< (this->blk_list).size() << std::endl;
    }
    std::cout << "333" << std::endl;
}

Segment::Segment(Flash flash, int seg_id, int start_sector_id, int pre_seg_size, int blk_per_seg, int sector_per_blk, int wearlimit, Segment * prev) {
    this->flash = flash;
    this->seg_id = seg_id;
    this->sector_offset = start_sector_id;
    this->pre_seg_size = pre_seg_size;
    this->sector_per_blk = sector_per_blk;
    
    time(&(this->version));
    this->maximum_size = blk_per_seg - pre_seg_size;
    this->remaining_size = blk_per_seg - pre_seg_size;
    this->next_free_blk_id = pre_seg_size;

    this->wearlimit = wearlimit;
    this->current_usage = 1;
    
    if (prev != NULL) {
        this->prev_seg_starting_sector = prev->get_start_sector();
    }
    else {
        this->prev_seg_starting_sector = NULL_BLOCK_INDEX;
    }
    this->next_seg_starting_sector = start_sector_id + blk_per_seg * sector_per_blk;

    for (int i = 0; i < blk_per_seg; i++) {
        int start = start_sector_id + (pre_seg_size+i) * sector_per_blk;
        std::cout << "333" << std::endl;
        Block * blk = new Block(this->flash, start, sector_per_blk, i+pre_seg_size, wearlimit, this->current_usage);
        (this->blk_list).push_back(*blk);
    }
}

Segment::~Segment() {
    (this->blk_list).clear();
}
Segment * Segment::get_prev() {
    // check if the prev is not the superblock
    if (this->remaining_size >= this->maximum_size) {
        return NULL;
    }
    else {
        int start = this->prev_seg_starting_sector;
        Segment * prev = new Segment(this->flash, start, this->pre_seg_size, this->blk_per_seg, this->sector_per_blk, this->wearlimit);
        return prev;
    }
}
Segment * Segment::get_next() {
    // check if it is the last segment
    if (this->remaining_size <= 0) {
        return NULL;
    }
    else {
        int start = this->next_seg_starting_sector;
        Segment * next = new Segment(this->flash, start, this->pre_seg_size, this->blk_per_seg, this->sector_per_blk, this->wearlimit);
        return next;
    }
}

Block * Segment::get_blk(int blk_id){
    if (blk_id < this->pre_seg_size) {
        return NULL;
    }
    else if (blk_id > this->maximum_size) {
        return NULL;
    }
    else{
        int id = blk_id - this->pre_seg_size;
//        if (blk_id <= this->next_free_blk_id) {
            return &((this->blk_list)[id]);
//        }
//        else {
//            return NULL;
//        }
    }
}

bool Segment::set_new_blk(int inode_id, int inode_v, char * s) {
    if (this->next_free_blk_id > this->maximum_size) {
        return FALSE;
    }
    else{
        int id = this->next_free_blk_id - this->pre_seg_size;
        Block * blk =new Block(this->flash, this->next_seg_starting_sector, this->sector_per_blk, id, this->wearlimit, this->current_usage);
        (this->blk_list)[id] = *blk;
        --(this->remaining_size);
        ++(this->next_free_blk_id);
        return ((this->blk_list)[id]).set_content(s, inode_id, inode_v);
    }
    
}

bool Segment::write_to_flash() {
    int len = sizeof(struct segment_metadata) + this->blk_per_seg * sizeof(seg_block);
    char * seg_metadata = (char *) malloc(len);
    memset (seg_metadata,0,len);
    ((struct segment_metadata *) seg_metadata)->seg_id = this->seg_id;
    ((struct segment_metadata *) seg_metadata)->version = this->version;
    ((struct segment_metadata *) seg_metadata)->current_usage = this->current_usage;
    ((struct segment_metadata *) seg_metadata)->remaining_size = this->remaining_size;
    ((struct segment_metadata *) seg_metadata)->prev_start_sector = this->prev_seg_starting_sector;
    ((struct segment_metadata *) seg_metadata)->next_start_sector = this->next_seg_starting_sector;
    
    
    char * blk_list = seg_metadata + sizeof(struct segment_metadata);
    int blk_in_used = this->blk_per_seg - this->remaining_size - this->pre_seg_size;
    for (int i = 0; i < blk_in_used; i++) {
        char * element = ((this->blk_list)[i]).to_string();
        memcpy(blk_list, element, sizeof(seg_block));
        blk_list += sizeof(seg_block);
    }
    std::cout << this->sector_offset << std::endl;
    print_t(seg_metadata);
    return Flash_Write(this->flash, this->sector_offset, this->pre_seg_size, seg_metadata);
}

int Segment::block_reach_wearlimit() {
    for(std::vector<Block>::size_type i = 0; i != this->blk_list.size(); i++) {
        if (((this->blk_list)[i]).get_remaining_usage() == 0) {
            return i;
        }
    }
    return NULL_BLOCK_INDEX;
}

