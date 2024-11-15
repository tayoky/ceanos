#pragma once

#define MAX_FILES 100
#define MAX_FILE_SIZE 1024
#define MAX_FILE_NAME 32

#include <stdint.h>
#include <util.h>
#include <list.h>
#include <types.h>


typedef struct {
        char name[MAX_FILE_NAME];
        char data[MAX_FILE_SIZE];
        unsigned int size;
} File;

typedef struct {
        File files[MAX_FILES];
        int file_count;
} VFS;

struct vfs_node_struct ;
typedef inode;

//functions
typedef ssize_t (*read_type_t) (struct fs_node *,  off_t, size_t, uint8_t *);
typedef ssize_t (*write_type_t) (struct fs_node *, off_t, size_t, uint8_t *);
typedef void (*open_type_t) (struct fs_node *, unsigned int flags);
typedef void (*close_type_t) (struct fs_node *);
typedef int (*create_type_t) (struct fs_node *, char *name, mode_t permission);
typedef int (*unlink_type_t) (struct fs_node *, char *name);
typedef int (*mkdir_type_t) (struct fs_node *, char *name, mode_t permission);
typedef int (*chmod_type_t) (struct fs_node *, mode_t mode);
typedef int (*chown_type_t) (struct fs_node *, uid_t, gid_t);
typedef int (*set_size_type_t) (struct fs_node *,size_t);

typedef struct vfs_node_struct {
        char name[256];
        uid_t owner;
        gid_t group_owner;
        size_t size;
        inode inode;
        struct vfs_node_struct *parent;
        struct vfs_node_struct *brother;
        time_t create_time;
        time_t acces_time;
        time_t modify_time;
        int32_t ref_count;
        int32_t driver;
        read_type_t read;
        write_type_t write;
        open_type_t open;
        close_type_t close;
        create_type_t create;
        mkdir_type_t mkdir;
        unlink_type_t unlink;
        set_size_type_t set_size;
        chown_type_t chown;
        chmod_type_t chmod;
} vfs_node ;