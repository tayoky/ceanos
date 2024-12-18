//@brief /dev/zero device creator
#pragma once

#include "vfs.h"

void create_zero_dev();
zero_open(vfs_node *node);
zero_close(vfs_node *node);
zero_read(vfs_node *node,uint32_t offset,uint32_t count,void *buffer);
zero_write(vfs_node *node,uint32_t offset,uint32_t count,void *buffer);
