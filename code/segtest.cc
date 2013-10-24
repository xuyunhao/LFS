#include "segment.h"
#include "flash.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main(){
	uint flash_size;
	char * mydisk = "my_test_disk";
	if(!Flash_Create(mydisk, 1000, (1024/FLASH_SECTORS_PER_BLOCK))){
		std::cout << "flash creation failed, could be file already exists" << std::endl;
	}
	
	std::cout << "111111" << std::endl;
	Flash flash = Flash_Open(mydisk, FLASH_SILENT, &flash_size);
	if(&flash == NULL){
		std::cout << "flash open failed." << std::endl;
	}
	printf("expected size: %d\n", flash_size);
	if(flash_size != (1024/FLASH_SECTORS_PER_BLOCK)){
		std::cout << "flash size differ from what's expected"<<std::endl;
	}
	Flash fptr = &flash;
	//pre seg in blocks
	int pre_seg_size = (sizeof(segment_metadata)%FLASH_SECTOR_SIZE) ? sizeof(segment_metadata)/FLASH_SECTOR_SIZE+1: sizeof(segment_metadata)/FLASH_SECTOR_SIZE;
	Segment * test_seg = new Segment(flash, 1, 0, pre_seg_size, 32, FLASH_SECTORS_PER_BLOCK, 1000, NULL);
	std::cout << "222222" << std::endl;
	test_seg->set_new_blk(0, 1, mydisk);
	if(test_seg->write_to_flash()) {
        std::cout << "333333" << std::endl;
    }
    Flash_Close(flash);
    fptr = Flash_Open(mydisk, FLASH_SILENT, & flash_size);
	std::cout << "333333" << std::endl;
	Segment * test_read = new Segment(fptr, 0, pre_seg_size, 32, FLASH_SECTORS_PER_BLOCK, 1000);
	std::cout << "444444" << std::endl;
	std::cout << test_read->get_blk(0)->to_string() << std::endl;
	return 0;
}
