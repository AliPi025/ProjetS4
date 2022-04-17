/********************************************************************
 *    Couche 1 : les blocs                                          *
 * 	  Gestion de l'écriture d'un bloc sur le disque                 *
 * 	  Projet S4                                                     *
 *                                                                  *
 * 	  K.Mohamed - Univ. Toulouse III Paul-Sabatier   2021-2022      *
 *                                                                  *
 ********************************************************************/
 
 
#include "couche1.h"


void init_disk_sos(char* nom_repertoire,virtual_disk_t virtual_disk_sos)
{
    char* file_emplacement = "./";
    strcat(file_emplacement,nom_repertoire);
    strcat(file_emplacement,"/d0");
    virtual_disk_sos.storage = fopen(file_emplacement,"ab+");
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

unsigned int compute_nblock(unsigned int n_octets){
	
	unsigned int nb_blocs = 0 ;
	
	
	if ( n_octets == 0 ){
		printf("ERREUR compute_nblock : Nombre d'octets donnes est null \n");
		return  0 ;
	}
	
	/**
	 * Dans le cas ou le nombre d'octets est pairs
	 */
	 
	if (n_octets%2==0 ){
		/* Des blocs de 4 octets */
		if ( n_octets >= 4 ){
			while ( n_octets!=0 ){
				nb_blocs++ ;
				n_octets= n_octets - 4 ;
			}
		}
		if (n_octets == 2) {
			/* Dans le cas ou le nombre d'octets = 2 octets -> 1 blocs */
			nb_blocs++ ;
		}
	}
		 
	
	/**
	 * Dans le cas ou le nombre d'octets est impairs
	 */
	 
	 if (n_octets%2!=0 ){
		 if ( n_octets > 4 ){
			 int q = n_octets / 4 ;
			 /* Des blocs de 4 octets dans n octets */
			 for(int i=0 ; i < q ; i++){
				 nb_blocs++ ;
			 }
			 /* Ajouter un bloc pour les octets qui reste ' inférieur à 4 octets '*/
			 nb_blocs++;
		}
		
		else {
			/* On les mets dans un seul bloc de n octets */
			nb_blocs++;
		}
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
	
	
	/**
	 *  On doit fermer le fichier du système après la fin de l'écriture du bloc
	 */
	 
	 eteindreSys(virtual_disk_sos.storage);
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

