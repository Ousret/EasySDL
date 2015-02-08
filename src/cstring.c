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

/**
 * \file cstring.c
 * \brief EasySDL: Gestion personnalise des chaines de caracteres
 * \author TAHRI Ahmed
 * \version 0.2
 * \date 08-02-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 * Les fonctions suivantes s'utilise dans un contexte particulier, à utiliser avec des pincettes.
 *
 */

#include <string.h>
#include "ESDL.h"

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
	
	dest[i] = '\0';
	
}

void formatedcpy(char *dst, char *src, size_t srclen) {

	if (!(dst || src)) return;
	int i = 0;
	
	for (i = 0; i < (int) srclen; i++) {
		
		if ((src[i] >= 'a' && src[i] <= 'z') || (src[i] >= 'A' && src[i] <= 'Z') || (src[i] == '\0') || (src[i] == ' ')) {
			dst[i] = src[i];
		}else{
			src[i] = '\0';
			break;
		}

	}
	
	if (src[i] != '\0') dst[i+1] = '\0';
	
}