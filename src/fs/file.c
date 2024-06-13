#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "kernel.h"

struct filesystem* filesystems[RCOS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[RCOS_MAX_FILE_DESCRIPTORS];

static struct filesystem** fs_get_free_filesystem() 
{
    for (int i = 0; i < RCOS_MAX_FILESYSTEMS; i++) {
        if (filesystems[i] == 0) {
            return &filesystems[i];
        }
    }

    return 0;
};

void fs_insert_filesystem(struct filesystem* filesystem)
{
    struct filesystem** fs;
    if (filesystem == 0) {
        print("Null filesystem provided");
        while(1){}
    }

    fs = fs_get_free_filesystem();
    if (!fs) {
        print("Could not insert FS");
        while(1){}
    }

    *fs = filesystem; //inserts into the array
};

static void fs_static_load()
{
    fs_insert_filesystem(x());
}

void fs_load(){
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init() 
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor** desc_out)
{
    int res = -ENOMEM;
    for (int i = 0; i < RCOS_MAX_FILE_DESCRIPTORS; i++) {
        if (file_descriptors[i] == 0) {
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            desc->index = i+1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;

        }
    }

    return res;
}

static struct file_descriptor* file_get_descriptor(int fd) 
{
    if (fd <= 0 || fd >= RCOS_MAX_FILE_DESCRIPTORS) {
        return 0;
    }

    return file_descriptors[fd -1];
}

struct filesystem* fs_resolve(struct disk* disk) 
{
    struct filesystem* fs = 0;
    for (int i = 0; i < RCOS_MAX_FILESYSTEMS; i++) {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0) {
            fs = filesystems[i];
            break;
        }
    }

    return fs;
}

int fopen(const char* filename, const char* mode)
{
    return -EIO;
}
