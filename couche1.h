/********************************************************************
 *    Couche 1 : les blocs                                          *
 *    Gestion de l'écriture d'un bloc sur le disque                 *
 *    Projet S4                                                     *
 *                                                                  *
 *    K.Mohamed - Univ. Toulouse III Paul-Sabatier  2021-2022       *
 *                                                                  *
 ********************************************************************/


#ifndef couche1_h
#define couche1_h


#include "sos_defines.h"
#include "include.h"
#include "timestamp.h"


/* Initialise la variable 'virtual_disk_sos' à partir du nom du répertoire */
void init_disk_sos(char* nom_repertoire,virtual_disk_t virtual_disk_sos);

/* Fonction qui veille a ce que le système soit éteint proprement : Sauvgarde du catalogue */
void eteindreSys(FILE* vdisk);

/* Calcule le nombre de blocs nécessaires pour stocker un nombre n_octets */
unsigned int compute_nblock(unsigned int n_octets);

/* Écrit un bloc block à la position pos sur le disque du système */
void write_block(long pos,virtual_disk_t virtual_disk_sos);

/* Lit un bloc block à la position pos sur le disque du système */
void read_block(long pos,virtual_disk_t virtual_disk_sos);

#endif

