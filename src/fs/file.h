#ifndef FILE_H
#define FILE_H 
#include "pparser.h"

typedef unsigned int FILE_SEEK_MODE;
enum {
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef unsigned int FILE_MODE;
enum {
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;
typedef void*(*FS_OPEN_FUNCTION)(struct disk* disk, struct path_part, FILE_MODE mode);
typedef int (*FS_RESOLVE_FUNCTION)(struct disk* disk); // checks if it can be read with FAT16

// represents a type of filesystem eg FAT or NTFS 
struct filesystem
{
    FS_RESOLVE_FUNCTION resolve; // should return 0 if disk is using file system    
    FS_OPEN_FUNCTION open;
    char name[20]; 
};

struct file_descriptor 
{
    int index;
    struct filesystem* filesystem;
    void* private; // used by FAT harddisk
    struct disk* disk;
};

void fs_init();
int fopen(const char* filename, const char* mode);
void fs_insert_filesystem(struct filesystem* filesystem);
struct filesystem*  fs_resolve(struct disk* disk);

#endif