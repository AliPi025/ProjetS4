#ifndef couche2_h
#define couche2_h

#include "sos_defines.h"
#include "timestamp.h"

/* Ecrit les infos de la variable super_block dans le super_block du disque */
void write_super_block(FILE *storage, super_block_t super_block);

/* Rentre les infos du super_block dans la variable super_block */
void read_super_block(FILE *storage, super_block_t *super_block);

/* Récupère les infos de la table d'inodes du disque */
void read_inodes_table(FILE *storage, inode_table_t tables_inodes);

/* Ecrit la table d'inodes sur le disque */
void write_inodes_table(FILE *storage, inode_table_t table_inodes);

/* Permet de retirer un inode (desc d'un fichier) de la table et de la mettre à jour */
void delete_inode(FILE *storage, int indice);

/* Retourne l'indice du premier inode disponible de la table */
int get_unused_inode(FILE *storage);

/* Initialise un inode grâce au fd, nom du fichier, taille, position et user id */
void init_inode(FILE *storage, char nom_fichier[FILENAME_MAX_SIZE], uint taille, uint pos, session_t session);

/* Affiche sur la sortie les infos de chaque inode de la table */
void cmd_dump_inode(FILE *storage);

#endif