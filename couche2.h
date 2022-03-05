#ifndef couche2_h
#define couche2_h

#include "sos_defines.h"
#include "timestamp.h"

/* J'ai écrit la couche 2 mais j'ai changé 2/3 truc par rapport au sujet:
        - J'utilise fd, pread/pwrite que je trouve largement plus simple à manier que FILE, fread/fwrite,
        donc à chaque fois je demande le fd à la place du FILE 
        - Dans init_inode je demande aussi la session_t parce que sans ça je ne vois pas comment je peux 
        initialiser l'uid de l'inode
        - Je demande à nouveau le fd dans cmd_dump_inode à la place du repertoire du fichier/disque.
        - Je fais afficher toutes les inodes avec cette dernière je ne sais pas si c'est exactement ce qu'elle doit faire */

// A voir si on décide que nos fonctions void renvoient ou non un code d'erreur/validation à l'exécution

/* Ecrit les infos de la variable super_block dans le super_block du disque */
void write_super_block(int fd, super_block_t super_block);

/* Rentre les infos du super_block dans la variable super_block */
void read_super_block(int fd, super_block_t *super_block);

/* Met a jour le champ first_free_byte du super_block */
void update_first_free_byte(int fd);

/* Récupère les infos de la table d'inodes du disque */
void read_inodes_table(int fd, inode_table_t tables_inodes);

/* Ecrit la table d'inodes sur le disque */
void write_inodes_table(int fd, inode_table_t table_inodes);

/* Permet de retirer un inode (desc d'un fichier) de la table et de la mettre à jour */
void delete_inode(int fd, int indice);

/* Retourne l'indice du premier inode disponible de la table */
int get_unused_inode(int fd);

/* Initialise un inode grâce au fd, nom du fichier, taille, position et user id */
void init_inode(int fd, char nom_fichier[FILENAME_MAX_SIZE], uint taille, uint pos, session_t session);

/* Affiche sur la sortie les infos de chaque inode de la table */
void cmd_dump_inode(int fd);

#endif