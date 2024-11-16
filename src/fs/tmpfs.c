#include <fs/vfs.h>
typedef struct{
    char name[256];
    inode *parent;
    inode *child;
    inode *brother;
    int size;
    int type;
}inode;

int init_tmpfs(){
    //create node
    vfs_node *root;
    root = tmpfs_create_dir(NULL);
    //mount as root
}
vfs_node *
 tmpfs_create_dir(inode *parent){
    vfs_node *node=kmalloc(sizeof(vfs_node));
    node->inode = kmalloc(sizeof(inode));
    node->permmision = 0777;
    
    node->inode->parent = parent;
    node->inode->child = NULL
    node->inode->brother = NULL
    if(parent){
        node->inode->brother = parent->child;
        parent->child = node->inode;
    }

    node->mkdir = tmpfs_mkdir;
    return node;
}


int tmpfs_mkdir(vfs_node node,char *name){
    vfs_node *dir =tmpfs_create_dir(node->inode);
    strcpy(dir->inode->name,name);
    return 0;
}

int tmpfs_open(vfs_node node){
    return 0;
}

int tmpfs_close(vfs_node node){
    kfree(node->inode);
}


