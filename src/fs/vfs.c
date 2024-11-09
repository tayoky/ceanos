#include "vfs.h"
#include <mm/mem.h>
#include <stdint.h>
#include <strings.h>
#include <stdlib/stdio.h>

void init_vfs(VFS *vfs)
{       
        vfs->file_count = 0;
        debugf("[vfs] virtual fs done!\n");
}

void create_file(VFS *vfs, const char *filename, const char *data)
{
        if (vfs->file_count > MAX_FILES) {
                return -1;
        }

        File *file = &vfs->files[vfs->file_count];
        int name_len = strlen(filename);
        int data_len = strlen(data);

        memcpy(file->name, filename, name_len+1);
        memcpy(file->data, data, data_len+1);

        file->size = data_len;
        vfs->file_count++;
}

const char* read_file(VFS* vfs, const char *filename)
{
        for (int i=0;i<vfs->file_count;i++){
                if(strcmp(vfs->files[i].name, filename) != 0){
                        return vfs->files[i].data;
                }
        }
        debugf("\n[vfs] file not found: %s\n", filename);
}

void delete_file(VFS *vfs, const char *filename)
{
        File replacement;
        for (int i=0;i<vfs->file_count;i++){
                if(strcmp(vfs->files[i].name, filename) != 0){
                        vfs->files[i] = replacement;
                        vfs->file_count--;
                }
        }
        debugf("\n[vfs] file not found: %s\n", filename);
}
