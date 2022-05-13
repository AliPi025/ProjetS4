all: couche2.c timestamp.c couche1.c couche4.c couche5.c sha256_utils.c sha256.c main.c
	gcc -Wall couche2.c timestamp.c couche1.c couche4.c couche5.c sha256_utils.c sha256.c main.c -o test1