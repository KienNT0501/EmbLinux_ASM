#include "shared_memory.h"
#include "production_info.h"
int main(){
    int fd = shm_open((char*)FILENAME, O_RDWR, 0666);
    block = (info*)mapping_memory_block(fd);
    if (block == NULL) return 1;
    printf("Getting data...\n");
    sleep(2);
    printf("READER read: ID: %s, name: %s, price: %d\n", block->ID, block->name, block->price);
     //Clean
    unmapping_memory_block(block, BLOCK_SIZE);
    unlink_memory_block((char*)FILENAME, fd);
}