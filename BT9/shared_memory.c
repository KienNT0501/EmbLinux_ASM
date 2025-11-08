#include "shared_memory.h"
#define IPC_RESULT_ERROR (-1)

void* mapping_memory_block(int fd){
        void* addr = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) {
            perror("mmap failed");
        }
        return addr;
}

bool unmapping_memory_block(void* addr, int size){
    return (munmap(addr, size) != IPC_RESULT_ERROR);
}

bool unlink_memory_block(char* filename, int fd) {
    close(fd);
    return (shm_unlink(filename) != IPC_RESULT_ERROR);
}
