#ifndef couche4_h
#define couche4_h

#include "sos_defines.h"

uint write_file(char filename[], file_t fichier);

uint read_file(char filename[], file_t *fichier);

uint delete_file(char filename[]);

void load_file_from_host(char filename[]);

void store_file_to_host(char filename[]);

#endif