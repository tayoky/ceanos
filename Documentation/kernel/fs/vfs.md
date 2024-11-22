## WARNING !

### *THIS ARTICLE IS A WORK IN PROGRESS.*

### > As a result, it may not contain all the information you need. 

# Ceanos VFS

The CeanOS VFS works just like a regular UNIX-like vfs.  
It is node based, meaning that every file, directory, mount point, etc is just a `vfs_node`

## VFS structs

### `vfs_node`

Contains:

- **Name** (256 bytes character array)
- **Owner** (`uid_t`)
- **Group owner** (`gid_t`)
- **Size** (`size_t`)
- **Inode** (pointer to `inode_struct`)
- **Children count** (`uint32_t`)
- **Child** (pointer to first child `vfs_node`)
- **Parent** (pointer to parent `vfs_node`)
- **Brother** (pointer to sibling `vfs_node`)
- **Create time** (`time_t`)
- **Access time** (`time_t`)
- **Modify time** (`time_t`)
- **Reference count** (`uint32_t`)
- **Driver** (`uint32_t`)
- **Read** (function pointer of type `read_type_t`)
- **Write** (function pointer of type `write_type_t`)
- **Open** (function pointer of type `open_type_t`)
- **Close** (function pointer of type `close_type_t`)
- **Create** (function pointer of type `create_type_t`)
- **Mkdir** (function pointer of type `mkdir_type_t`)
- **Unlink** (function pointer of type `unlink_type_t`)
- **Set size** (function pointer of type `set_size_type_t`)
- **Chown** (function pointer of type `chown_type_t`)
- **Chmod** (function pointer of type `chmod_type_t`)
- **Readdir** (function pointer of type `readdir_type_t`)
- **Finddir** (function pointer of type `finddir_type_t`)
- **Permissions** (`mode_t`)
- **Type** (`uint8_t`)

### dirrent struct (defined in `types.h`)

Contains:

- **name** (256 bytes character array)

## Other VFS stuff

```c
#define VFS_NODE_TYPE_FILE 0x01 
#define VFS_NODE_TYPE_FOLDER 0x02
#define VFS_NODE_TYPE_MOUNT_POINT 0x04
#define VFS_NODE_TYPE_PIPE 0x08
#define VFS_NODE_TYPE_SYMLINK 0x10
#define VFS_NODE_TYPE_CHAR_DEVICE 0x20
```

## VFS functions
The CeanOS virtual file system utilizes the following functions:

### `vfs_node *kopen(char *path)`
Open a node at the specified path an return it. 

#### *NOTE* : the path must be an absolute path

#### *NOTE* : after opening don't forget to close using `vfs_close`

### `int vfs_close(vfs_node *node)`
Close the specifed node and free it if needed/posible

### `ssize_t vfs_read(vfs_node *node, off_t offset, size_t count, void *buffer)`

Execute read of the specifed node  

#### *NOTE* : read isn't available on all nodes, if the node is a directory it will return `ERR_CANT_READ` or `ERR_NOT_A_FILE` (Defined in `errno.h`)

### `ssize_t vfs_write(vfs_node *node, off_t offset, size_t count, void *buffer)`

#### work i progress article

### `struct dirrent *vfs_readdir(vfs_node *node, uint32_t index)`

Read a directory entry from a specified VFS node. It uses the readdir function pointer of the node to fetch the directory entry at the given index. If the node does not have a readdir function, the function will return NULL.

#### Parameters:

- node: A pointer to the directory node from which we want to read.
- index: The index (or offset) within the directory, indicating which directory entry to read.

#### Return:

- If node->readdir is set it will return a dirrent(directory entry) pointer for the specified index.

- If node->readdir is NULL, it returns NULL (reading the directory entry isn't supported by the node).
