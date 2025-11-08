#include "shared_memory.h"
#include "production_info.h"
static void usageError (const char * progName){
    fprintf(stderr, "Usage: %s ID name price\n", progName);
    exit(EXIT_FAILURE);
}
int main(int argc, char*argv[]){
    if(argc != 4 ) usageError(argv[0]);
    int flags = O_CREAT | O_RDWR;
    int fd = shm_open((char*)FILENAME, flags, 0666);
        if (fd == -1){
            perror("shm_open");
        }
        if (ftruncate(fd, BLOCK_SIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
        }
    //Map Shared memory
        block = (info*)mapping_memory_block(fd);
    //Asign value (Note: USE strcpy() NOT manualy assign element: X block->ID = argv[1] X)
        strcpy(block->ID, argv[1]);
        strcpy(block->name, argv[2]);
        block->price = atoi(argv[3]);
    // Notify has written
        printf("WRITER wrote: ID: %s, name: %s, price: %d\n", block->ID, block->name, block->price);
    //Clean
        // unmapping_memory_block(block, BLOCK_SIZE);
        // unlink_memory_block((char*)FILENAME, fd);   
}