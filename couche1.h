#ifndef couche1_h
#define couche1_h


#include "sos_defines.h"
#include "headers.h"
#include "include.h"


/* Initialise la variable 'virtual_disk_sos' à partir du nom du répertoire */
void init_disk_sos(char* nom_repertoire) ;


/* Fonction qui veille a ce que le système soit éteint proprement : Sauvgarde du catalogue */
void eteindreSys(FILE* vdisk);

/* Calcule le nombre de blocs nécessaires pour stocker un nombre n_octets */
unsigned int compute_nblock(unsigned int n_octets);
