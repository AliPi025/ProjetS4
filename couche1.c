/********************************************************************
 *    Couche 1 : les blocs                                          *
 * 	  Gestion de l'écriture d'un bloc sur le disque             *
 * 	  Projet S4                                                 *
 *                                                                  *
 * 	  K.Mohamed - Univ. Toulouse III Paul-Sabatier              *
 *                                                                  *
 /******************************************************************/
 
 
#include "headers.h"
#include "include.h"


virtual_disk_t virtual_disk_sos ;

void init_disk_sos(char* nom_repertoire)
{

    char* file_emplacement = "./";
    strcat(file_emplacement,nom_repertoire);
    strcat(file_emplacement,"/d0");
    virtual_disk_sos.storage = fopen(file_emplacement,"ab+");
    virtual_disk_sos.super_block = {(uint)0,(uint)0,(uint)0,(uint)0};
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
		if (nb_octets == 2) {
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
