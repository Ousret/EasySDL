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

char * getstringpart(char * text, char elem, int indice) {
	
	char * result = NULL;
	int cpt = 1, start_ind = 0, size_dst = 0, i = 0;
	
	if (cpt > indice) {
		
		for (i = 0; i < (strlen(text)); i++) {
			if (text[i] == '\n') {
				cpt++;
				if (cpt == indice) {
					start_ind = i;
					break;
				}
			}
		}
		
		if (cpt > indice) return NULL;
		
	}
	
	i = start_ind;
	
	while (text[i] != '\n' && text[i] != '\0') {
		
		if (!result) {
			result = malloc(sizeof(char));
		}else{
			result = realloc(result, sizeof(char)*(size_dst+1));
		}
		
		result[size_dst] = text[i];
		size_dst++;
		i++;
		
	}
	
	return result;
	
}