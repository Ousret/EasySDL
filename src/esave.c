/*
*	Encrypted personal data
*	public domain code; OpenSSL EVP apis.
*	Started by Saju Pillai (saju.pillai@gmail.com)
*	Changed, adapted & improved by TAHRI Ahmed (@Ousret)
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#include "esave.h"

EVP_CIPHER_CTX en, de;
unsigned int salt[] = {12345, 54321};
unsigned char key_data[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
const int key_data_len = 32;
FILE *savfile = NULL;

int initAES() {
	
	if (aes_init(key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
    	return 0;
	}
	
	return 1;
}

d_save * initProfil(char * filename) {
/* Load profil file or start new one */

	d_save * tmp = NULL;
	char param[256] , value[256];
	savfile = fopen(filename, "r");
	/* Allocate and init new save bloc */
	tmp = malloc(sizeof(d_save));
	tmp->filename = filename;
	tmp->data = NULL;
	tmp->elem = 0;
	
	/* If "filename" could'nt be readed, just alloc new zone.. Save Later ! */
	if (!savfile) return tmp;
	
	while (fgets(param, 256, savfile) && fgets(value, 256, savfile)) {
		
		if (tmp->data) {
			tmp->data = realloc(tmp->data, sizeof(d_param)*((tmp->elem) + 1));
		}else{
			tmp->data = malloc(sizeof(d_param));
		}
		
		tmp->data[tmp->elem].param = malloc(sizeof(unsigned char)*256);
		tmp->data[tmp->elem].value = malloc(sizeof(unsigned char)*256);
		
		param[strlen(param)-2] = 0;
		value[strlen(param)-2] = 0;
		
		fprintf(stdout, "Param = %s\nValue = %s\n", param, value);
		
		strcpy((char*) tmp->data[tmp->elem].param, param);
		strcpy((char*) tmp->data[tmp->elem].value, value);
		
		tmp->elem = (tmp->elem)+1;
		
	}
	
	fclose(savfile);
	return tmp;
}

char * readParam(d_save * profil, char * param) {

	if (!profil) return NULL;
	int i;
	int len = (int) strlen(param)+1, olen = (int) strlen(param)+1;
	char * plaintext;
	
	initAES();
	
	for (i = 0; i < (profil->elem); i++) {
		
		plaintext = (char *)aes_decrypt(&de, profil->data[i].param, &len);
		if (!strncmp(plaintext, param, olen)) {
			free (plaintext);
			plaintext = (char *)aes_decrypt(&de, profil->data[i].value, &len);
			EVP_CIPHER_CTX_cleanup(&de);
			return plaintext;
		}
		free (plaintext);
		
	}
	
	return NULL;
}

int writeParam(d_save * profil, char * param, char * value) {
	if (!profil) return 0;
	int i;
	int len = (int) strlen(param)+1, olen = (int) strlen(param)+1;
	char * plaintext;
	
	initAES();
	
	//Check if param is already in here.
	for (i = 0; i < (profil->elem); i++) {
		
		plaintext = (char *)aes_decrypt(&de, profil->data[i].param, &len);
		
		fprintf(stdout, "Uncrypt = %s\n", plaintext);
		
		if (!strncmp(plaintext, param, olen)) {
			free (plaintext);
			strcpy((char*) profil->data[i].value, (char *)aes_encrypt(&en, (unsigned char*)value, &len));
			EVP_CIPHER_CTX_cleanup(&en);
			return 1;
		}
		
		free (plaintext);
		
	}
	
	//If not create new one.
	if (profil->data) {
		profil->data = realloc(profil->data, sizeof(d_param)*((profil->elem)+1));
	}else{
		profil->data = malloc(sizeof(d_param));
	}
	
	profil->data[profil->elem].param = malloc(sizeof(unsigned char)*256);
	profil->data[profil->elem].value = malloc(sizeof(unsigned char)*256);
	
	strcpy((char*) profil->data[profil->elem].param, (char *)aes_encrypt(&en, (unsigned char*)param, &len));
	strcpy((char*) profil->data[profil->elem].value, (char *)aes_encrypt(&en, (unsigned char*)value, &len));
	
	profil->elem = (profil->elem)+1;
	
	return 1;
}

int saveProfil(d_save * profil) {
	
	if (!profil) return 0;
	if (!(profil->filename)) return 0;
	
	int i = 0;
	
	savfile = fopen(profil->filename, "w");
	if (!savfile) return 0;
	
	for (i = 0; i < (profil->elem); i++) {
		
		fprintf(savfile, "%s\n%s\n", profil->data[i].param, profil->data[i].value);
		
	}
	
	fclose(savfile);
	return 1;
	
}

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, 
             EVP_CIPHER_CTX *d_ctx)
{
	int i, nrounds = 5;
	unsigned char key[32], iv[32];
  
	/*
	* Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
	* nrounds is the number of times the we hash the material. More rounds are more secure but
	* slower.
	*/
	
	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
	
	if (i != 32) {
		printf("Key size is %d bits - should be 256 bits\n", i);
		return -1;
	}

	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	
	return 0;
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
	/* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
	int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
	unsigned char *ciphertext = malloc(c_len);
	
	/* allows reusing of 'e' for multiple encryption cycles */
	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
	
	/* update ciphertext, c_len is filled with the length of ciphertext generated,
	*len is the size of plaintext in bytes */
	EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);
	
	/* update ciphertext with the final remaining bytes */
	EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);
	
	*len = c_len + f_len;
	return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
	/* plaintext will always be equal to or lesser than length of ciphertext*/
	int p_len = *len, f_len = 0;
	unsigned char *plaintext = malloc(p_len);
	  
	EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
	EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);
	
	*len = p_len + f_len;
	return plaintext;
}
