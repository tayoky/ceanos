#include "vfs.h"
#include <mm/mem.h>
#include <stdint.h>
#include <strings.h>
#include <stdlib/stdio.h>
#include <mm/malloc.h>
#include <errno.h>

vfs_node *vfs_root_node = NULL;

int vfs_int(){
    if(vfs_root_node !=NULL){
        printf("vfs aready INIT !!!!\n");
        return -1;
    }

    //allocate space for the root node
    vfs_root_node = kmalloc(sizeof(vfs_node));

    //i don't belive there are any kind of time in this kernel for the moment so
    vfs_root_node->create_time = 0;
    vfs_root_node->acces_time = 0;
    vfs_root_node->modify_time = 0;

    //set all propreties
    vfs_root_node->owner =0;
    vfs_root_node->group_owner = 0;

    vfs_root_node->childreen_count = 0;
    vfs_root_node->ref_count -1;

    vfs_root_node->driver = 0;

    vfs_root_node->permmision = 0777;
    vfs_root_node->type =0;

    vfs_root_node->parent = NULL;
    vfs_root_node->brother = NULL;

    vfs_root_node->read = NULL;
    vfs_root_node->write = NULL;
    vfs_root_node->open = NULL;
    vfs_root_node->close = NULL;
    vfs_root_node->create = NULL;
    vfs_root_node->mkdir = NULL;
    vfs_root_node->unlink = NULL;
    vfs_root_node->set_size = NULL;
    vfs_root_node->chown = NULL;
    vfs_root_node->chmod = NULL;

    strcmp("root",vfs_root_node->name);
}

struct dirrent *vfs_readdir(vfs_node *node,uint32_t index){
    if(node->readdir){
        return node->readdir(node,index);
    } else {
        return NULL;
    }
}

struct vfs_node_struct *vfs_finddir(vfs_node *node,char *name){
    //first check for special path
    //the self path
    if(!strcmp(name,VFS_SPECIAL_PATH_SELF))>
        return node;
    }
    //the parent path
    if(!strcmp(name,VFS_SPECIAL_PATH_PARENT){
        return node->parent;
    }
    //let check if node is adready in memory

    vfs_node *current_node = node->child;
    for(uint32_t i=0;i < node->childreen_count;i++){
        //check the name
        if(!strcmp(current_node->name,name)){
            //we find it ! just return it
            return current_node;
        }

        //go to next child
        current_node = current_node->brother;
    }

    //it isen't in memory so 
    if(!node->finddir){
        return NULL;
    }

    vfs_node *ret = node->finddir(node,name);
    if((ret != NULL )&& (node->ref_count != -1)){
        node->ref_count ++;
    }

    return ret;
}
ssize_t vfs_read(vfs_node *node,off_t offset,size_t count,void *buffer){
    if(node->read){
        return node->read(node,offset,count,buffer);
    } else  {
        return ERR_NO_EVAL;
    }
}
ssize_t vfs_write(vfs_node *node,off_t offset,size_t count,void *buffer){
    if(node->write){
        return node->write(node,offset,count,buffer);
    } else  {
        return ERR_NO_EVAL;
    }
}
int vfs_open(vfs_node *node){
    if(node->open){
        return node->open(node);
    } else  {
        return ERR_NO_EVAL;
    }
}
int vfs_close(vfs_node *node){
    if(node->ref_count == -1){
        //it's lock don't do anything
        return 0;
    }

    node->ref_count --;
    if(node->ref_count == 0){
        if(node->close){
            node->close(node);
        }
    }
}
int vfs_create(vfs_node *node,char *name,mode_t permission);
int vfs_mkdir(vfs_node *node,char *name,mode_t permission);
int vfs_unlink(vfs_node *node,char *name);
int vfs_set_size(vfs_node *node,size_t new_size);
int vfs_chown(vfs_node *node,uid_t user,gid_t group);
int vfs_chmod(vfs_node *node,mode_t permission);