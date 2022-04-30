/********************************************************************
 *    Couche 1 : les blocs                                          *
 *    Gestion de l'écriture d'un bloc sur le disque                 *
 *    Projet S4                                                     *
 *                                                                  *
 *    K.Mohamed - Univ. Toulouse III Paul-Sabatier   2021-2022      *
 *                                                                  *
 ********************************************************************/
 
 
#include "couche1.h"


void init_disk_sos(char* nom_repertoire,virtual_disk_t virtual_disk_sos)
{
    char* file_emplacement = "./";
    strcat(file_emplacement,nom_repertoire);
    strcat(file_emplacement,"/d0");
    virtual_disk_sos.storage = fopen(file_emplacement,"rb+");
    //virtual_disk_sos.super_block = {(uint)0,(uint)0,(uint)0,(uint)0};
    /* A voir
    virtual_disk_sos.users_table
    */
}

void eteindreSys(FILE* vdisk)
{
    fclose(vdisk);
    //A Completer

}

unsigned int compute_nblock(uchar n_octets){
	
	uchar nb_blocs = 0 ;
	
	
	if ( n_octets == 0 ){
		printf("ERREUR compute_nblock : Nombre d'octets donnes est null \n");
		return  0 ;
	}
	
	while ( n_octets > 0 ){
		nb_blocs++ ;
		n_octets= n_octets - BLOCK_SIZE;
	}
	return nb_blocs ;
}


void write_block(long pos,virtual_disk_t virtual_disk_sos){
	
	/**
	 *  On commence par positioner le curseur à la position donnée en paramètre
	 * 	-> On considère que le fichier du virtual_disk_sos est ouvert dans init_disk_sos(char* nom_repertoire) 
	 */
	 
	int position = 0 ;
	position = fseek(virtual_disk_sos.storage,pos,SEEK_SET);
	if (position != 0) {
		fprintf(stderr, "Position impossible - fonction ecriture\n");
		eteindreSys(virtual_disk_sos.storage);
		exit(EXIT_FAILURE);
	}
	
	/**
	 *  La variable buffer sera le bloc de mémoire dans lequel sont stockés les octetsà écrire
	 */
	 
	block_t buffer ;
	if (fwrite(buffer.data,BLOCK_SIZE,1,virtual_disk_sos.storage )!=1) {
		fprintf(stderr, "Ecriture du bloc impossible dans le fichier\n");
		eteindreSys(virtual_disk_sos.storage);
		exit(EXIT_FAILURE);
	}
	
}


void read_block(long pos,virtual_disk_t virtual_disk_sos){
	
	/**
	 *  On commence par positioner le curseur à la psotion 'pos'
	 *  -> On considère que le fichier du virtual_disk_sos est ouvert dans init_disk_sos(char* nom_repertoire) 
	 */
	
	int position = 0 ;
	position = fseek(virtual_disk_sos.storage,pos,SEEK_SET);
	if (position != 0) {
		fprintf(stderr, "Position impossible - fonction lecture\n");
		eteindreSys(virtual_disk_sos.storage);
		exit(EXIT_FAILURE);
	}
	
	block_t buffer ;
	if (fread(buffer.data,BLOCK_SIZE,1,virtual_disk_sos.storage )!=1) {
		fprintf(stderr, "Lecture du bloc impossible dans le fichier\n");
		eteindreSys(virtual_disk_sos.storage);
		exit(EXIT_FAILURE);
	}
}

