#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file-path>\n", argv[0]);
        return 1;
    }

    struct stat sb;

    // Call lstat() instead of stat()
    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        return 1;
    }
    //File path
    printf("Input File Path: %s \n", argv[1]);
    // File type
    printf("File type: ");
    if (S_ISREG(sb.st_mode))  printf("regular file\n");
    else if (S_ISDIR(sb.st_mode)) printf("directory\n");
    else if (S_ISLNK(sb.st_mode)) printf("symbolic link\n");
    else printf("other\n");

    // Size
    printf("Size: %ld bytes\n", sb.st_size);

    // Timestamps
    printf("Last access:       %s", ctime(&sb.st_atime));
    printf("Last modification: %s", ctime(&sb.st_mtime));
    printf("Last status change:%s", ctime(&sb.st_ctime));

    return 0;
}

