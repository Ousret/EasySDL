

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "esave.h"
#include "aes.h"
#include "db_lite.h"
#include "cstring.h"

d_save * initProfil(char * filename) {
/* Load profil file or start new one */
	
	if (!db_open(filename)) return NULL;
	d_save * tmp = NULL;
	unsigned char ** sqlAnswer = NULL;
	char * Rparam = NULL;
	int sizesqlAnser = 0, i = 0;
	
	/* Allocate and init new save bloc */
	tmp = malloc(sizeof(d_save));
	tmp->filename = filename;
	tmp->data = NULL;
	tmp->elem = 0;
	
	while (readText(db, i, sqlAnswer, &sizesqlAnser)) {
		
		Rparam = malloc(sizeof(char)*(strlen((char*)sqlAnswer[0])+1));
		strcpy(Rparam, (char*) sqlAnswer[0]);
		
		readBlob(db, Rparam, sqlAnswer, &sizesqlAnser);
		writeParam(tmp, Rparam, (char*) sqlAnswer[0]);
		
		free(Rparam);
		
		i++;
	}
	
	/* If "filename" could'nt be readed, just alloc new zone.. Save Later ! */
	//if (!savfile) return tmp;
	
	//fclose(savfile);
	return tmp;
}

char * readParam(d_save * profil, char * param) {

	if (!profil) return NULL;
	int i;
	int len = (int) strlen(param)+1, olen = (int) strlen(param)+1;
	char * plaintext = NULL;
	
	for (i = 0; i < (profil->elem); i++) {
		
		//plaintext = (char *)aes_decrypt(&de, profil->data[i].param, &len);
		if (!strncmp(plaintext, param, olen)) {
			//free (plaintext);
			//plaintext = (char *)aes_decrypt(&de, profil->data[i].value, &len);
			//EVP_CIPHER_CTX_cleanup(&de);
			return plaintext;
		}
		//free (plaintext);
		
	}
	
	return NULL;
}

int writeParam(d_save * profil, char * param, char * value) {
	
	if (!profil) return 0;
	int i;
	int len = (int) strlen(param)+1, olen = (int) strlen(param)+1;
	char * plaintext = NULL;
	unsigned char *encryptedval = NULL;
	

	//Check if param is already in here.
	for (i = 0; i < (profil->elem); i++) {
		
		/* If we have a match.. */
		if (!strcmp(profil->data[i].param, param) == 0) {
			
			encryptedval = aes_encrypt(&en, (unsigned char*)value, &len);
			unsignedchar_memcpy(profil->data[i].value, encryptedval, sizeof(unsigned char)*len);
			
			return 1;
		}
		
	}
	
	//If not create new one.
	if (profil->data) {
		profil->data = realloc(profil->data, sizeof(d_param)*((profil->elem)+1));
	}else{
		profil->data = malloc(sizeof(d_param));
	}
	
	encryptedval = aes_encrypt(&en, (unsigned char*)value, &len);
	
	profil->data[profil->elem].param = malloc(sizeof(char)*strlen(param)+1);
	profil->data[profil->elem].value = malloc(sizeof(unsigned char)*sizeof(unsigned char)*len);
	
	memcpy (profil->data[profil->elem].param, param, strlen(param)+1);
	unsignedchar_memcpy(profil->data[profil->elem].value, encryptedval, sizeof(unsigned char)*len);
	
	profil->elem = (profil->elem)+1;
	
	return 1;
}

int saveProfil(d_save * profil) {
	
	if (!profil) return 0;
	if (!(profil->filename)) return 0;
	
	int i = 0;
	
	//savfile = fopen(profil->filename, "w");
	//if (!savfile) return 0;
	
	for (i = 0; i < (profil->elem); i++) {
		
		//fprintf(savfile, "%s\n%s\n", profil->data[i].param, profil->data[i].value);
		
	}
	
	//fclose(savfile);
	return 1;
	
}
