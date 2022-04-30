/********************************************************************
 *    Couche 4 : les blocs                                          *
 *    Gestion de l'écriture d'un bloc sur le disque                 *
 *    Projet S4                                                     *
 *                                                                  *
 *    K.Mohamed - Univ. Toulouse III Paul-Sabatier   2021-2022      *
 *                                                                  *
 ********************************************************************/

#ifndef couche4_h
#define couche4_h

/* Fonction qui écrit le fichier 'fichier' sur le système */
void write_file(char* Nomfichier,file_t fichier)


#include "sos_defines.h"
#include "timestamp.h"
