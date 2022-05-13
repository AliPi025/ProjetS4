#include "couche2.h"
#include "couche1.h"

extern virtual_disk_t virtual_disk_sos;
extern session_t session;

/* Ecrit les infos de la variable super_block dans le super_block du disque */
void write_super_block(FILE *storage, super_block_t super_block){
    block_t *block = &super_block;
    for(int i = 0; i < SUPER_BLOCK_SIZE; i++){
        write_block(*block, i*BLOCK_SIZE);
        block++;
    }
}

/* Rentre les infos du super_block dans la variable super_block */
void read_super_block(FILE *storage, super_block_t *super_block){
    block_t *block = super_block;
    for(int i = 0; i < SUPER_BLOCK_SIZE; i++){
        read_block(block, i*BLOCK_SIZE);
        block++;
    }
}

/* Met a jour le champ first_free_byte du super_block */
void update_first_free_byte(){
    int i = INODES_START+INODE_TABLE_SIZE*INODE_SIZE*BLOCK_SIZE;
    block_t read_b;
    read_block(&read_b, i);
    while(read_b.data[0] != 0 && read_b.data[1] != 0 && read_b.data[2] != 0 && read_b.data[3] != 0){
        i += BLOCK_SIZE;
        read_block(&read_b, i);
    }
    if(read_b.data[0] == 0);
    else if(read_b.data[1] == 0) i++;
    else if(read_b.data[2] == 0) i+=2;
    else if(read_b.data[3] == 0) i+=3;
    virtual_disk_sos.super_block.first_free_byte = i;
}

/* Récupère les infos de la table d'inodes du disque */
void read_inodes_table(FILE *storage, inode_table_t table_inodes){
    super_block_t super_block;
    read_super_block(storage, &super_block);
    fseek(storage, INODES_START, SEEK_SET);
    for(int i = 0; i<super_block.number_of_files; i++){
        fread(&table_inodes[i], BLOCK_SIZE, INODE_SIZE, storage);
    }
}

/* Ecrit la table d'inodes sur le disque */
void write_inodes_table(FILE *storage, inode_table_t table_inodes){
    super_block_t super_block;
    read_super_block(storage, &super_block);
    fseek(storage, INODES_START, SEEK_SET);
   for(int i = 0; i<super_block.number_of_files; i++){
       fwrite(&table_inodes[i], BLOCK_SIZE, INODE_SIZE, storage);
   }
}

/* Permet de retirer un inode (desc d'un fichier) de la table et de la mettre à jour */
void delete_inode(int indice){
    virtual_disk_sos.super_block.nb_blocks_used -= virtual_disk_sos.inodes[indice].nblock;
    for(int i = indice; i+1<virtual_disk_sos.super_block.number_of_files; i++){
      virtual_disk_sos.inodes[i] = virtual_disk_sos.inodes[i+1];
    }
    virtual_disk_sos.super_block.number_of_files--;
}

/* Retourne l'indice du premier inode disponible de la table */
int get_unused_inode(){
    return (int) virtual_disk_sos.super_block.number_of_files;
}

/* Initialise un inode grâce au fd, nom du fichier, taille, position et user id */
void init_inode(char nom_fichier[FILENAME_MAX_SIZE], uint taille, uint pos){
    inode_t inode;
    char *date;
    strcpy(inode.filename, nom_fichier);
    inode.size = taille;
    inode.first_byte = pos;
    inode.uid = session.userid;
    inode.uright = RW;
    inode.oright = rw;
    inode.nblock = (inode.size+BLOCK_SIZE-1)/BLOCK_SIZE;
    date = timestamp();
    strcpy(inode.ctimestamp, date);
    strcpy(inode.mtimestamp, date);
    virtual_disk_sos.inodes[get_unused_inode()] = inode;
    virtual_disk_sos.super_block.number_of_files++;
    virtual_disk_sos.super_block.nb_blocks_used += inode.nblock;
}

/* Affiche sur la sortie les infos de chaque inode de la table */
void cmd_dump_inode(){
    for(int i = 0; i<virtual_disk_sos.super_block.number_of_files; i++){
        inode_t inode = virtual_disk_sos.inodes[i];
        printf("Inode %d: \n", i);
        printf("    - Nom: %s\n", inode.filename);
        printf("    - Taille: %d\n", inode.size);
        printf("    - User ID: %d\n", inode.uid);
        printf("    - User Rights: %d\n", inode.uright);
        printf("    - Others Rights: %d\n", inode.oright);
        printf("    - Date Création: %s\n", inode.ctimestamp);
        printf("    - Date Modification: %s\n", inode.mtimestamp);
        printf("    - Nombre Blocks: %d\n", inode.nblock);
        printf("    - Premier Octet: %d\n\n", inode.first_byte);
    }
}



// Affichage attendu:
/*
DiskDir/d0
Inode 0: 
    - Nom: test
    - Taille: 25
    - User ID: 0
    - User Rights: 3
    - Others Rights: 0
    - Date Création: Sat Mar  5 17:24:04 2022 <-------------- Bon évidemment la date change
    - Date Modification: Sat Mar  5 17:24:04 2022
    - Nombre Blocks: 7
    - Premier Octet: 46

Inode 1: 
    - Nom: test2
    - Taille: 35
    - User ID: 0
    - User Rights: 3
    - Others Rights: 0
    - Date Création: Sat Mar  5 17:24:04 2022
    - Date Modification: Sat Mar  5 17:24:04 2022
    - Nombre Blocks: 9
    - Premier Octet: 67

Inode 0: 
    - Nom: test2
    - Taille: 35
    - User ID: 0
    - User Rights: 3
    - Others Rights: 0
    - Date Création: Sat Mar  5 17:24:04 2022
    - Date Modification: Sat Mar  5 17:24:04 2022
    - Nombre Blocks: 9
    - Premier Octet: 67

*/