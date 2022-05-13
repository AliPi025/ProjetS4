/*******************************************************************
*    Couche 1 : les blocs                                          *
*    Gestion de l'écriture d'un bloc sur le disque                 *
*    Projet S4                                                     *
*                                                                  *
*    K.Mohamed - Univ. Toulouse III Paul-Sabatier   2021-2022      *
*                                                                  *
*******************************************************************/

#include "couche1.h"
#include "couche2.h"

extern virtual_disk_t virtual_disk_sos;

void init_disk_sos(char nom_repertoire[])
{
    char file_emplacement[50] = "./";
    strcat(file_emplacement,nom_repertoire);
    strcat(file_emplacement,"/d0");
    virtual_disk_sos.storage = fopen(file_emplacement,"rb+");
    //read_super_block(virtual_disk_sos.storage, &(virtual_disk_sos.super_block));
	//read_inodes_table(virtual_disk_sos.storage, virtual_disk_sos.inodes);
    /* A voir
    virtual_disk_sos.users_table
    */
}

void shut_system()
{
	write_super_block(virtual_disk_sos.storage, virtual_disk_sos.super_block);
	write_inodes_table(virtual_disk_sos.storage, virtual_disk_sos.inodes);
	fclose(virtual_disk_sos.storage);
    //A Completer

}

uint compute_nblock(uint n_octets){
	return (n_octets+BLOCK_SIZE-1)/BLOCK_SIZE;
}


void write_block(block_t block, long pos){
	
	/**
	 *  On commence par positioner le curseur à la position donnée en paramètre
	 * 	-> On considère que le fichier du virtual_disk_sos est ouvert dans init_disk_sos(char* nom_repertoire) 
	 */
	fseek(virtual_disk_sos.storage, pos, SEEK_SET);
	fwrite(&block,BLOCK_SIZE, 1, virtual_disk_sos.storage);
}


void read_block(block_t *block, long pos){
	
	/**
	 *  On commence par positioner le curseur à la psotion 'pos'
	 *  -> On considère que le fichier du virtual_disk_sos est ouvert dans init_disk_sos(char* nom_repertoire) 
	 */
	fseek(virtual_disk_sos.storage,pos,SEEK_SET);
	
	if (fread(block,BLOCK_SIZE,1,virtual_disk_sos.storage )!=1) {
		fprintf(stderr, "Lecture du bloc impossible dans le fichier\n");
		shut_system();
		exit(EXIT_FAILURE);
	}
}

