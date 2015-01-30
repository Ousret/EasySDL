/**
 * \file cstring.c
 * \brief EasySDL: extended string func
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.1
 * \date 30-01-2015
 *
 * cstring
 *
 */

#include <string.h>
#include <stdlib.h>

#include "cstring.h"

long getcharocc(char * text, char elem) {
	long i = 0, cpt = 0;
	
	for (i = 0; i < (strlen(text)); i++) {
		if (text[i] == elem) cpt++;
	} 
	
	return cpt;
} 

void replaceinstring(char * text, char elem, char newc) {
	if (!text || elem == newc) return;
	long i = 0;
	
	for (i = 0; i < (strlen(text)); i++) {
		if (text[i] == elem) text[i] = newc;
	} 
}