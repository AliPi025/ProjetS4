#include "couche1.h"
#include "couche2.h"

extern virtual_disk_t virtual_disk_sos;
extern session_t session;

void read_users_table(){
    block_t *block;
    block = &(virtual_disk_sos.users_table);
    char *passwordfile = "passwd";
    int j = 0;
    while(j < virtual_disk_sos.super_block.number_of_files && !strcmp(virtual_disk_sos.inodes[j].filename, passwordfile)){
        j++;
    }
    for(int i = 0; i < compute_nblock(sizeof(users_table_t)); i++){
        read_block(block, virtual_disk_sos.inodes[j].first_byte+i*BLOCK_SIZE);
        block++;
    }
}

void write_users_table(){
    block_t *block;
    block = &(virtual_disk_sos.users_table);
    char *passwordfile = "passwd";
    int j = 0;
    while(j < virtual_disk_sos.super_block.number_of_files && !strcmp(virtual_disk_sos.inodes[j].filename, passwordfile)){
        j++;
    }
    for(int i = 0; i < compute_nblock(sizeof(users_table_t)); i++){
        read_block(block, virtual_disk_sos.inodes[j].first_byte+i*BLOCK_SIZE);
        block++;
    }
}

void create_root(char login[], char password[]){
    strcpy(virtual_disk_sos.users_table[0].login, login);
    strcpy(virtual_disk_sos.users_table[0].passwd, password);
}