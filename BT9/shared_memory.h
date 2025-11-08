#include <stdio.h>
#include <fcntl.h> 
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SHARED_MEMORY_H
#include <stdbool.h>
#define BLOCK_SIZE 4096
#define FILENAME "/myshm"

void* mapping_memory_block(int fd);
bool unmapping_memory_block(void* addr, int size);
bool unlink_memory_block(char* filename, int fd);
