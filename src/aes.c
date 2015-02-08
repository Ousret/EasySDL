/**
 * \file aes.c
 * \brief EasySDL: Encryption / Decryption management AES 256bits
 * \author TAHRI Ahmed
 * \version 0.9
 * \date 08-02-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 * public domain code; OpenSSL EVP apis.
 * Started by Saju Pillai (saju.pillai@gmail.com)
 * Changed, adapted & improved by TAHRI Ahmed (@Ousret)
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ESDL.h"

EVP_CIPHER_CTX en, de; /* AES buffer, encryption + decrypt */
unsigned int salt[] = {12345, 54321};
const unsigned char key_data[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
const unsigned char *key_data_op = NULL;
unsigned int allowcustomkey = 0;
const int key_data_len = 32;

int aes_custom_32key(const unsigned char *key32) {
	
	if (!key32) return 0;
	key_data_op = key32;
	allowcustomkey = 1;
	
	return 1;
} 

void aes_clean() {
	EVP_CIPHER_CTX_cleanup(&en);
	EVP_CIPHER_CTX_cleanup(&de);
}

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int aes_init(
	unsigned char *key_data, 
	int key_data_len, 
	unsigned char *salt, 
	EVP_CIPHER_CTX *e_ctx, 
    EVP_CIPHER_CTX *d_ctx
    ) 
{
    
	int i, nrounds = 5;
	unsigned char key[32], iv[32];
  	if (allowcustomkey) key_data = (unsigned char*) key_data_op;
	/*
	* Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
	* nrounds is the number of times the we hash the material. More rounds are more secure but
	* slower.
	*/
	i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
	
	
	if (i != 32) {
		fprintf(stderr, "<! Error> EasySDL: Key size is %d bits - should be 256 bits\n", i);
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
	
	if (aes_init((unsigned char*)key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
    	return NULL;
	}
	
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
	
	aes_clean();
	
	return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
	
	if (aes_init((unsigned char*)key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
    	return NULL;
	}
	
	/* plaintext will always be equal to or lesser than length of ciphertext*/
	int p_len = *len, f_len = 0;
	unsigned char *plaintext = malloc(p_len);
	
	EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
	EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);
	
	*len = p_len + f_len;
	
	aes_clean();
	
	return plaintext;
}