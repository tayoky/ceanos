#pragma once
#include <fs/vfs.h>

int init_tmpfs();
vfs_node *tmpfs_create_dir(inode *parent);
int tmpfs_mkdir(vfs_node *node,char *name);
