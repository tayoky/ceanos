#include "vfs.h"
#include <mm/mem.h>
#include <stdint.h>
#include <strings.h>
#include <stdlib/stdio.h>
#include <mm/malloc.h>
#include <errno.h>
#include <kernel.h>

vfs_node *vfs_root_node;

int vfs_init(){
    if(vfs_root_node != NULL) {
        debugf("[vfs] vfs is already initialized !\n");
        return -1;
    }

    //allocate space for the root node
    vfs_root_node = kmalloc(sizeof(vfs_node));
    debugf("[vfs] allocated space for 'vfs_root_node'!\n");
    sleep(300);

    //i don't belive there is any kind of time in the kernel for the moment so
    vfs_root_node->create_time = 0;
    vfs_root_node->acces_time = 0;
    vfs_root_node->modify_time = 0;

    //set all propreties
    vfs_root_node->owner =0;
    vfs_root_node->group_owner = 0;

    vfs_root_node->childreen_count = 0;

    vfs_root_node->driver = 0;

    vfs_root_node->permission = 0777;
    vfs_root_node->type =0;
    
    //weird but on unix-like OS's the parent of root is root
    //if you're on linux try cd / and cd ..
    vfs_root_node->parent = vfs_root_node;
    vfs_root_node->brother = NULL;
    vfs_root_node->ref_count = 0;

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

    strcpy(vfs_root_node->name,"root");
    
    debugf("[vfs] done!\n");
    sleep(100);
    return SUCCESS;
}

struct dirrent *vfs_readdir(vfs_node *node ,uint32_t index){
    if(node->readdir){
        return node->readdir(node,index);
    } else {
        return NULL;
    }
}

struct vfs_node_struct *vfs_finddir(vfs_node *node , char *name) {
    //first check for special path
    //the self path
    if(!strcmp(name,VFS_SPECIAL_PATH_SELF)){
        return node;
    }
    //the parent path
    if(!strcmp(name,VFS_SPECIAL_PATH_PARENT)){
        return node->parent;
    }
    //let check if node is adready in memory

    vfs_node *current_node = node->child;
    for(uint32_t i=0;i < node->childreen_count;i++){
        //check the name
        if(!strcmp(current_node->name,name)){
            //we found it ! just return it
            return current_node;
        }

        //go to next child
        current_node = current_node->brother;
    }

    //it isn't in memory so 
    if(!node->finddir){
        return NULL;
    }

    vfs_node *ret = node->finddir(node,name);
    if((ret != NULL )&& (node->ref_count != -1)){
        node->childreen_count ++;
        ret->parent = node;
        ret->brother = node->child;
        node->child = ret;
        node->ref_count ++;
        //add in the children
    }

    return ret;
}

ssize_t vfs_read(vfs_node *node,off_t offset,size_t count,void *buffer){
    if(node->read){
        return node->read(node,offset,count,buffer);
    } else  {
        return ERR_CANT_READ;
    }
}

ssize_t vfs_write(vfs_node *node,off_t offset,size_t count,void *buffer){
    if(node->write){
        return node->write(node,offset,count,buffer);
    } else  {
        return ERR_CANT_WRITE;
    }
}

int vfs_open(vfs_node *node){
    if(!node)     return ERR_BAD_INPUT ;
    if(node->ref_count != -1)    node->ref_count ++;
    if(node->open)    return node->open(node);
}

int vfs_close(vfs_node *node){
    if(node->ref_count == -1){
        // it's locked so don't do anything
        return 0;
    }

    node->ref_count--;
    if(node->ref_count == 0){
        // check if the node's parent is not the root before recursively closing it
        if (node->parent != vfs_root_node) {
            vfs_close(node->parent);
        }

        if(node->close) {
            node->close(node);
        }
        
        if(node == vfs_root_node) {
            kfree(node);
            return 0;
        }
        
        // now remove the node from the chain
        vfs_node *current_node = node->parent->child;
        while(current_node->child != node){
            current_node = current_node->child;
        }
        kfree(node);
    }
}

int vfs_create(vfs_node *node, char *name, mode_t permission){
    if(node->mkdir){
        return node->create(node,name,permission);
    } else {
        return ERR_NOT_A_DIRECTORY;
    }
}

int vfs_mkdir(vfs_node *node, char *name, mode_t permission){
    if(node->mkdir){
        return node->mkdir(node,name,permission);
    } else {
        return ERR_NOT_A_DIRECTORY;
    }
}

int vfs_unlink(vfs_node *node, char *name);
int vfs_set_size(vfs_node *node, size_t new_size);
int vfs_chown(vfs_node *node, uid_t user, gid_t group);
int vfs_chmod(vfs_node *node, mode_t permission);

char **parse_path(char *path){
    //first count the number of depth
    uint32_t path_depth = 0;
    for(int i = 0; path[i] ; i++){
        //only if its a path separator
        if(path[i] != '/') {
            continue;
        }
        path[i] = '/0';
        if(path[i + 1]) {
            path_depth++;
        }
    }

    //alocate space for the array
    char **path_array = kmalloc(sizeof(char) *( path_depth + 1));
    path_array[path_depth] = NULL;

    int j = 0;
    for(int i = 0;i < path_depth; i++){
        while(path[j]){
            j++;
        }
        path_array[i] = j + 1;
    }
    return path_array;
}

vfs_node *kopen(char *path){
        //let open any file
        //check open is an abosulte path
        if(path[0] != '/'){
            //not an abosulte path
            debugf("[kopen] not an absolute path!\n");
            return NULL;
        }

        if (path == NULL) {
            printf("[kopen] path is NULL!\n");
            return NULL;
        }

        //first duplicate the path
        char *new_path = kmalloc(strlen(path) + 1);
        strcpy(new_path,path);
        path = new_path;

        //parse the path
        char **path_array = parse_path(path);
        
        vfs_node *current = vfs_root_node;
        for(int i=0;path_array[i]!=NULL;i++){
            //if null it's an error
            if(current == NULL){
                kfree(new_path);
                kfree(path_array);
                die("path_array is null\n", ERR_UNKNOW);
                return NULL;
            }
            current = vfs_finddir(current, path_array[i]);
        }
        kfree(new_path);
        kfree(path_array);
        if(current != NULL){
            vfs_open(current);
        }
        return current;
}
   
int vfs_mount(char *path, vfs_node *node) {
    //first let open the folder
    vfs_node *dest = kopen(path);
    debugf("[vfs mount] node [%p] succesfully opened!\n",dest);

    //if null error
    if(dest == NULL) {
        return die("error: no such file or directory", ERR_NO_FILE_OR_DIRECTORY);
    }
    
    //if it has child you can't mount
    if(dest->childreen_count){ 
        return die("error: not empty", ERR_NOT_EMPTY);
    }

    //if it used we can't mount
    if(dest->ref_count != 1) {
        return die("unknown error", ERR_UNKNOW);
    }
    
    //set the new node
    strcpy(node->name,dest->name);
    node->parent = dest->parent;
    node->brother = NULL;
    node->ref_count = -1;
    node->type |= VFS_NODE_TYPE_MOUNT_POINT;

    //now close the old node
    vfs_close(dest);
    debugf("[vfs mount] succefuly close node [%p]\n",dest);

    //special case we set root
    if(path[1] == '\0'){
       debugf("[vfs mount] mounting node [%p] as root\n",node);
       vfs_root_node = node;
       return 0;
    }

    //now make the last child point to us
    vfs_node *current_node = node->parent->child;
    while(current_node != NULL){
       current_node = current_node->brother;
    }
    current_node->brother = node;

    return 0;
}