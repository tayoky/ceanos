#pragma once
#include <fs/vfs.h>

int init_tmpfs();
vfs_node *tmpfs_create_dir(struct inode_struct inode * inode);
