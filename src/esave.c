/**
 * \file esave.c
 * \brief EasySDL: save module
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.2.1
 * \date 01-03-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 * esave.c est un module de sauvegarde
 * Au format couple (Parametre, Valeur)
 * Seul la valeur est crypte avec AES 256bits
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ESDL.h"

void SDL_freeProfil(d_save * profil) {
	
	if (!profil) return;
	int i = 0;
	
	for (i = 0; i < (profil->elem); i++) {
		if (profil->data[i].param) {
			free (profil->data[i].param);
			profil->data[i].param = NULL;
		} 
		if (profil->data[i].value) {
			free (profil->data[i].value);
			profil->data[i].value = NULL;
		} 
	}
	
	if (profil->data) {
		free(profil->data);
		profil->data = NULL;
	}
	
	profil->filename = NULL;
	
	free (profil);
	profil = NULL;
	
}

d_save * SDL_initProfil(char * filename) {
/* Load profil file or start new one */
	
	if (!db_open(filename)) return NULL;
	d_save * tmp = NULL;
	unsigned char * blobContainer = NULL;
	char * sqlKey = NULL, *aes_res = NULL;
	
	int sizesqlAnser = 0, i = 1;
	
	/* Allocate and init new save bloc */
	tmp = malloc(sizeof(d_save));
	tmp->filename = filename;
	tmp->data = NULL;
	tmp->elem = 0;
	
	while (readText(db, i, &sqlKey, &sizesqlAnser) == SQLITE_ROW) {
		
		readBlob(db, sqlKey, &blobContainer, &sizesqlAnser);
		if (!blobContainer) break; //Allocation failed.. need to avoid sigabrt
		
		aes_res = (char *)aes_decrypt(&de, blobContainer, &sizesqlAnser);
		SDL_writeParam(tmp, sqlKey, aes_res);
		
		/* Free local ptr */
		if (blobContainer) {
			free(blobContainer);
			blobContainer = NULL;
		}
		if (aes_res) {
			free (aes_res);
			aes_res = NULL;
		} 
		if (sqlKey) {
			free(sqlKey);
			sqlKey = NULL;
		}
		
		i++;
	}
	 
	db_close();
	return tmp;
}

char * SDL_readParam(d_save * profil, char * param) {

	if (!profil) return NULL;
	int len = (int) strlen(param)+1, i = 0;
	 
	for (i = 0; i < (profil->elem); i++) {
		 
		if (!strcmp(profil->data[i].param, param)) {
			 
			len = strlen((char*)profil->data[i].value)+1;
			return (char *)aes_decrypt(&de, profil->data[i].value, &len);
			
		}
		
	}
	
	return NULL;
}

int SDL_writeParam(d_save * profil, char * param, char * value) {
	
	if (!profil) return 0;
	int len = (int) strlen(value)+1, i = 0;
	unsigned char *encryptedval = NULL;
	 
	//Check if param is already in here.
	for (i = 0; i < (profil->elem); i++) {
		 
		/* If we have a match.. */
		if (!strcmp(profil->data[i].param, param)) {
			 
			encryptedval = aes_encrypt(&en, (unsigned char*)value, &len);
			if (!encryptedval) return 0; /* If nothing come out of aes_encrypt */
			
			/* Free old value from mem */
			if (profil->data[i].value) {
				free (profil->data[i].value);
				profil->data[i].value = NULL;
			} 
			/* Allocate new unsigned char* for value */
			profil->data[i].value = encryptedval;
			
			return 1;
		}
		
	}
	 
	//If not create new one.
	if (profil->data) {
		profil->data = realloc(profil->data, sizeof(d_param)*((profil->elem)+1));
	}else{
		profil->data = malloc(sizeof(d_param));
	}
	
	profil->data[profil->elem].param = NULL;
	profil->data[profil->elem].value = NULL;
	
	profil->data[profil->elem].param = malloc(sizeof(char)*(strlen(param)+1));
	profil->data[profil->elem].value = aes_encrypt(&en, (unsigned char*)value, &len);
	
	memcpy (profil->data[profil->elem].param, param, strlen(param)+1);
	profil->elem = (profil->elem)+1;
	
	return 1;
}

int SDL_saveProfil(d_save * profil) {
	
	if (!profil) return 0;
	if (!(profil->filename)) return 0;
	if (!db_open(profil->filename)) return 0;
	 
	int i = 0;
	dropBlobTable();
	createBlobTable();
	
	for (i = 0; i < (profil->elem); i++) {
		writeBlob(db, profil->data[i].param, profil->data[i].value, strlen((char*)profil->data[i].value));
	}
	 
	db_close();
	return 1;
	
}
