#pragma once

#include <fs/vfs.h>

int init_tmpfs();
int tmpfs_mkdir(vfs_node *node, char *name);
int tmpfs_open(vfs_node *node);
int tmpfs_close(vfs_node *node);

vfs_node* tmpfs_create_dir(inode *parent);
