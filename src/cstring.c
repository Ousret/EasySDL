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
#include "cstring.h"

long getcharocc(char * text, char elem) {
	unsigned long i = 0, cpt = 0;
	
	for (i = 0; i < (strlen(text)); i++) {
		if (text[i] == elem) cpt++;
	} 
	
	return cpt;
} 

void replaceinstring(char * text, char elem, char newc) {
	if (!text || elem == newc) return;
	unsigned long i = 0;
	
	for (i = 0; i < (strlen(text)); i++) {
		if (text[i] == elem) text[i] = newc;
	} 
}

void unsignedchar_memcpy(unsigned char *dest, unsigned char *src, size_t len) {
	
	unsigned long i = 0;
	if (!(dest || src)) return;
	
	for (i = 0; i < len; i++) {
		dest[i] = src[i];
	}
	
}