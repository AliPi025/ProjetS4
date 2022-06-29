# Projet S4 - L2 Informatique Paul Sabatier # 

                               //\\//\\//\\// Un systeme d'exploitation (tres) simplie UNIX like //\\//\\//\\//

Dans notre projet on a , un from scratch, c'est un systeme d'exploitation avec des fichiers, des utilisateurs, une procédure
de connexion, un interprète de commande. Le disque dur de la machine sera simule par un fichier qu'on
appellera le disque virtuel, qu'il faudra formater et qui contiendra les fichiers et le catalogue du disque.
Le systeme d'exploitation proprement dit sera un programme C, resident en mémoire, gerant des tables lui 
permettant de gerer un systeme de fichiers, qui sera écrit sur un disque dur simulé, pour le projet,
par un fichier.
Le systeme gerera diverses operations sur les fichiers, une table des utilisateurs, des droits (simplifies)
de ces derniers sur les fichiers, une procedure de connexion et un interprete de commande.
Comme il est d'usage en système, la logique de fonctionnement sera divisee en couches .

