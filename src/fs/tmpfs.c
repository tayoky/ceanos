#include <mm/malloc.h>
#include <errno.h>
#include <strings.h>

struct inode_struct;
typedef struct inode_struct{
    char name[256];
    struct inode_struct *parent;
    struct inode_struct *child;
    struct inode_struct *brother;
    uint32_t childreen_count;
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
    root = new_tmpfs();
    //mount as root
    int return_value = vfs_mount("/", root);

    printf("%d\n", return_value);
}

vfs_node *new_tmpfs(){
    inode *root_inode = tmpfs_new_inode(); 
    root_inode->type = TMPFS_TYPE_DIRECTORY;
    return tmpfs_inode_to_node(root_inode);
}

int tmpfs_mkdir(vfs_node *node, char *name, mode_t perm){
    //TODO: check if aready exist
    
    inode *folder_inode = tmpfs_new_inode();
    folder_inode->type = TMPFS_TYPE_DIRECTORY;
    folder_inode->parent = node->inode;
    folder_inode->brother = node->inode->child;
    node->inode->child = folder_inode;
    node->inode->childreen_count ++;
    return 0;
}

int tmpfs_create(vfs_node *node,char name,mode_t perm){
    inode *file_inode = tmpfs_new_inode();
    file_inode->type = TMPFS_TYPE_FILE;
    file_inode->parent = node->inode;
    file_inode->brother = node->inode->child;
    node->inode->child = file_inode;
    node->inode->childreen_count ++;
}

int tmpfs_open(vfs_node *node){
    return 0;
}

int tmpfs_close(vfs_node *node){
    return 0;
}

struct dirrent *tmpsfs_readdir(vfs_node *node,uint32_t index){
    //first the . path
    if(index == 0){
        struct dirrent *ret = kmalloc(sizeof(struct dirrent));
        strcpy(ret->name,VFS_SPECIAL_PATH_SELF);
        return ret;
    }

    //first the .. path
    if(index == 1){
        struct dirrent *ret = kmalloc(sizeof(struct dirrent));
        strcpy(ret->name,VFS_SPECIAL_PATH_PARENT);
        return ret;
    }

    index -= 2;
    inode *current = node->inode->child;
    for(uint32_t i=0;i<index;i++){
        if(current == NULL) return NULL;
        current = current->brother;
    }

    struct dirrent *ret = kmalloc(sizeof(struct dirrent));
    strcpy(ret->name,current->name);
}

vfs_node *tmpfs_finddir(vfs_node *node,char *name){
    //first let find the good inode
    inode *current = node->inode->child;
    while (strcmp(current->name,name))
    {
        if(current = NULL) return NULL;
        current = current->brother;
    }

    return tmpfs_inode_to_node(current);
}

vfs_node *tmpfs_inode_to_node(inode *og_inode){
    vfs_node *node = kmalloc(sizeof(vfs_node));
    
    node->inode = og_inode;
    node->permmision = 0777;
    node->childreen_count = 0;
    
    if(og_inode->type == TMPFS_TYPE_FILE){
        node->type= VFS_NODE_TYPE_FILE;
        node->open = tmpfs_open;
        node->close = tmpfs_close;
    } else if(og_inode->type == TMPFS_TYPE_DIRECTORY){
        node->type = VFS_NODE_TYPE_FOLDER;
        node->readdir = tmpsfs_readdir;
        node->finddir = tmpfs_finddir;
        node->create =tmpfs_create;
        node->mkdir = tmpfs_mkdir;
    }
}

inode *tmpfs_new_inode(){
    inode *node = kmalloc(sizeof(node));
    node->brother = NULL;
    node->child = NULL;
    node->parent = NULL;
    node->name[0] = '\0';
    node->childreen_count = 0;
    return node;
}