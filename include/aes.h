/* Only for OS X 10.9 or greater, APPLE UNIX ONLY */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <openssl/aes.h>
#include <openssl/evp.h>

extern EVP_CIPHER_CTX en, de;
extern unsigned int salt[];
extern const unsigned char key_data[];
extern const int key_data_len;

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);
void aes_clean();
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
int aes_custom_32key(const unsigned char *key32);