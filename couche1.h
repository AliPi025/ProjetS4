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

void init_disk_sos(char nom_repertoire[]);

void shut_system();

uint compute_nblock(uint n_octets);

void write_block(block_t block, long pos);

void read_block(block_t *block, long pos);

#endif

