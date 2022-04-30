/********************************************************************
 *    Couche 4 : les blocs                                          *
 * 	  Gestion de l'écriture d'un bloc sur le disque             *
 * 	  Projet S4                                                 *
 *                                                                  *
 * 	  K.Mohamed - Univ. Toulouse III Paul-Sabatier   2021-2022  *
 *                                                                  *
 ********************************************************************/
 
 
#include "couche4.h"
#include "couche2.h"
#include "couche1.h"


write_file(char* Nomfichier,file_t fichier){
	
	/*
	* Récuperer les informations de fichier de la table d'inodes et Super_block
	*/
	
	inode_table_t inode_Table ; 
	super_block_t super_Block ;
	char* date ;
	fpos_t *position ;

	read_inodes_table(virtual_disk_sos.storage,inode_Table);
	
	read_super_block(virtual_disk_sos.storage,super_Block);
	
	
	/* 
	 * Rechercher le nom de ficheir dans la table des inodes 
	*/
	
	//Taille des inodes des fichiers
	uint tailleMax = super_Block.number_of_files ;
	
	
	for(int i=0 ; i < tailleMax ;i++)
	{
		if(inode_Table[i].filename == Nomfichier )
		{
			/*
			 * Dans le cas ou le fichier est de taille inférieur ou égale
			 * --> Mettre à jour la table du inode
			 */
			
			if (inode_Table[i].size >= fichier.size )
			{
				inode_Table[i].size = fichier.size ;
				date = timestamp();
				inode_Table[i].mtimestamp = date ;
				inode_Table[i].nblock = compute_nblock(fichier.size);
				fseek(virtual_disk_sos.storage,inode_Table[i].first_byte,SEEK_SET);
				fgetpos(virtual_disk_sos.storage,position);
			}
			
			/*
			 * Dans le cas ou le fichier est de taille supérieur 
			 * --> Suppression du inode et création d'un nouveau à la fin du disque
			 */
			 
			else if (inode_Table[i].size < fichier.size)
			{
				delete_inode(virtual_disk_sos.storage,i);
				fseek(virtual_disk_sos.storage,0,SEEK_END);
				fgetpos(virtual_disk_sos.storage,position);
				init_inode(virtual_disk_sos.storage , Nomfichier ,fichier.size , (uint)position ) ;
			}	
		}
		
		else 
		{
			fseek(virtual_disk_sos.storage,0,SEEK_END);
			fgetpos(virtual_disk_sos.storage,position);
			init_inode(virtual_disk_sos.storage , Nomfichier ,fichier.size , (uint)position ) ;
		}
		
		/*
		* Ecriture des Blocks 
		*/
			 
		for(int j=0 , j < compute_nblock(fichier.size) , j++ )
		{
			write_block((uint)position) ;
			position = position+BLOCK_SIZE ;
		}
		
}

