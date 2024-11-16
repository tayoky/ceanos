#include <mm/malloc.h>

struct inode_struct;
typedef struct inode_struct{
    char name[256];
    struct inode_struct *parent;
    struct inode_struct *child;
    struct inode_struct *brother;
    int size;
    int type;
}inode;

#define TMPFS_TYPE_DIRECTORY 1
#define TMPFS_TYPE_FILE 2

#define VFS_DRIVER

#include "tmpfs.h"
#include <fs/vfs.h>

int init_tmpfs(){
    // create node
    vfs_node *root;
    root = tmpfs_create_dir(NULL);

    //mount as root
    vfs_mount("/",root);
}

vfs_node *tmpfs_create_dir(inode *parent){
    vfs_node *node=tmpfs_create_file(parent);

    node->mkdir = tmpfs_mkdir;
    node->type = VFS_NODE_TYPE_FOLDER;
    node->inode->type = TMPFS_TYPE_DIRECTORY;
    return node;
}

vfs_node *tmpfs_create_file(inode *parent){
    vfs_node *node=kmalloc(sizeof(vfs_node));
    node->inode = kmalloc(sizeof(inode));
    node->permmision = 0777;
    
    node->inode->type = TMPFS_TYPE_FILE;
    node->inode->parent = parent;
    node->inode->child = NULL;
    node->inode->brother = NULL;
    node->inode->size = 0;
    
    node->size = 0;
    if(parent){
        node->inode->brother = parent->child;
        parent->child = node->inode;
    }
}

int tmpfs_mkdir(vfs_node *node,char *name){
    vfs_node *dir = tmpfs_create_dir(node->inode);
    strcpy(dir->inode->name,name);
    return 0;
}

int tmpfs_open(vfs_node *node){
    return 0;
}

int tmpfs_close(vfs_node *node){
    kfree(node->inode);
    kfree(node);  
    return 0;
}

struct dirrent tmpsfs_readdir(vfs_node *node,uint32_t index){
    //first the . path
    if(index == 0){
        struct dirrent *ret = kmalloc(sizeof(struct dirrent));
        strcpy(ret->name,VFS_SPECIAL_PATH_SELF);
    }

    //first the .. path
    if(index == 1){
        struct dirrent *ret = kmalloc(sizeof(struct dirrent));
        strcpy(ret->name,VFS_SPECIAL_PATH_PARENT);
    }

    index -= 2;
    for(uint32_t i=0;i<index;i++){
        
    }
}