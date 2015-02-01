/*
*	Encrypted personal data
*	public domain code; OpenSSL EVP apis.
*	Started by Saju Pillai (saju.pillai@gmail.com)
*	Changed, adapted & improved by TAHRI Ahmed (@Ousret)
*/

/* Only for OS X APPLE */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

typedef struct {
	
	unsigned char *param;
	unsigned char *value;
	
} d_param;

typedef struct {
	
	char * filename;
	d_param * data;
	int elem;
	
} d_save;

extern EVP_CIPHER_CTX en, de;
extern unsigned int salt[];
extern unsigned char key_data[];
extern const int key_data_len;
extern FILE *savfile;

int saveProfil(d_save * profil);
int writeParam(d_save * profil, char * param, char * value);
char * readParam(d_save * profil, char * param);
d_save * initProfil(char * filename);
int initAES();

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
