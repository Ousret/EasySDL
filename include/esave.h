/*
*	Encrypted personal data
*	public domain code; OpenSSL EVP apis.
*	Started by Saju Pillai (saju.pillai@gmail.com)
*	Changed, adapted & improved by TAHRI Ahmed (@Ousret)
*/


typedef struct {
	
	char *param;
	unsigned char *value;
	
} d_param;

typedef struct {
	
	char * filename;
	d_param * data;
	int elem;
	
} d_save;

int saveProfil(d_save * profil);
int writeParam(d_save * profil, char * param, char * value);
char * readParam(d_save * profil, char * param);
d_save * initProfil(char * filename);


