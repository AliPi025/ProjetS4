#include "couche2.h"

/* Ecrit les infos de la variable super_block dans le super_block du disque */
void write_super_block(int fd, super_block_t super_block){
    pwrite(fd, &super_block, sizeof(super_block_t), 0);
}

/* Rentre les infos du super_block dans la variable super_block */
void read_super_block(int fd, super_block_t *super_block){
    pread(fd, super_block, sizeof(super_block_t), 0);
}

/* Met a jour le champ first_free_byte du super_block */
void update_first_free_byte(int fd){
    super_block_t super_block;
    read_super_block(fd, &super_block);
    super_block.first_free_byte = sizeof(super_block_t)+sizeof(inode_t)*super_block.number_of_files;
}

/* Récupère les infos de la table d'inodes du disque */
void read_inodes_table(int fd, inode_table_t tables_inodes){
    super_block_t super_block;
    read_super_block(fd, &super_block);
    for(int i = 0; i<super_block.number_of_files; i++){
        pread(fd, &tables_inodes[i], sizeof(inode_t), sizeof(super_block_t)+sizeof(inode_t)*i);
    }
}

/* Ecrit la table d'inodes sur le disque */
void write_inodes_table(int fd, inode_table_t table_inodes){
    super_block_t super_block;
    read_super_block(fd, &super_block);
   for(int i = 0; i<super_block.number_of_files; i++){
      pwrite(fd, &table_inodes[i], sizeof(inode_t), sizeof(super_block_t)+sizeof(inode_t)*i);
   }
}

/* Permet de retirer un inode (desc d'un fichier) de la table et de la mettre à jour */
void delete_inode(int fd, int indice){
    super_block_t super_block;
    int i;
    inode_t inode;
    read_super_block(fd, &super_block);
    pread(fd, &inode, sizeof(inode_t), sizeof(super_block_t)+sizeof(inode_t)*indice);
    super_block.nb_blocks_used -= inode.nblock;
    for(i = indice; i<super_block.number_of_files-1; i++){
        pread(fd, &inode, sizeof(inode_t), sizeof(super_block_t)+sizeof(inode_t)*(i+1));
        pwrite(fd, &inode, sizeof(inode_t), sizeof(super_block_t)+sizeof(inode_t)*i);
    }
    super_block.number_of_files--;
    write_super_block(fd, super_block);
}

/* Retourne l'indice du premier inode disponible de la table */
int get_unused_inode(int fd){
    super_block_t super_block;
    read_super_block(fd, &super_block);
    return (int) super_block.number_of_files;
}

/* Initialise un inode grâce au fd, nom du fichier, taille, position et user id */
void init_inode(int fd, char nom_fichier[FILENAME_MAX_SIZE], uint taille, uint pos, session_t session){
    inode_t inode;
    inode_table_t table_inodes;
    super_block_t super_block;
    char *date;
    strcpy(inode.filename, nom_fichier);
    inode.size = taille;
    inode.first_byte = pos;
    inode.uid = session.userid;
    inode.uright = RW;
    inode.oright = rw;
    inode.nblock = (inode.size+BLOCK_SIZE-1)/BLOCK_SIZE;///compute_nblock(taille); //<---------------------------------------- à checker
    date = timestamp();
    strcpy(inode.ctimestamp, date);
    strcpy(inode.mtimestamp, date);
    read_super_block(fd, &super_block);
    read_inodes_table(fd, table_inodes);
    table_inodes[get_unused_inode(fd)] = inode;
    super_block.number_of_files++;
    super_block.nb_blocks_used += inode.nblock;
    write_super_block(fd, super_block);
    write_inodes_table(fd, table_inodes);
    update_first_free_byte(fd);
}

/* Affiche sur la sortie les infos de chaque inode de la table */
void cmd_dump_inode(int fd){
    int i;
    super_block_t super_block;
    inode_t inode;
    read_super_block(fd, &super_block);
    for(i = 0; i<super_block.number_of_files; i++){
        pread(fd, &inode, sizeof(inode_t), sizeof(super_block_t)+sizeof(inode_t)*i);
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

/* main de test à compiler avec timestamp.c */
int main(void){
    int fd, cr;
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "echec fork\n");
        exit(1);
        break;
    case 0:
        execl("cmd_format", "./cmd_format", "DiskDir", "500000", NULL); // <------------------ il faut que le répertoire DiskDir existe déjà et que cmd_format ait été compilé !
        fprintf(stderr, "echec disk\n");
        exit(2);
        break;
        
    default:
        wait(&cr);
        break;
    }
    fd = open("./DiskDir/d0", O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
    // Initialise le super block, la table d'inodes et la session
    super_block_t sup_b = {0, 1, 0, sizeof(super_block_t)};
    inode_table_t tab_i;
    session_t ses = {0};
    // Ecrit le super block et la table d'inodes
    write_super_block(fd, sup_b);
    write_inodes_table(fd, tab_i);
    // initialise 2 inodes
    init_inode(fd, "test", 25, 46, ses);
    init_inode(fd, "test2", 35, 67, ses);
    // Affiche la table
    cmd_dump_inode(fd);
    // Retire la première inode
    delete_inode(fd, 0);
    // Affiche la table
    cmd_dump_inode(fd);
    close(fd);
    return 0;
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