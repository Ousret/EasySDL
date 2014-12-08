/**
 * \file STJ_SDL.h
 * \brief Librairies SDL/C 1.2
 * \author TAHRI Ahmed, SIMON Jérémy, DEZERE Florian, PROVOST Valentin
 * \version 0.2
 * \date 27 Octobre 2014
 *
 * Toutes les fonctions utiles la SDL
 *
 */
 
#define N 10
#define M 50

typedef enum {

	NUMERIC,
	VARCHAR,
	NUMCHAR,
	ALL
	
} t_typeForm;

typedef struct {

	int type;
	int x;
	int y;
	int height;
	int width;
	char title[50];
	char * dest; //Only for form obj
	t_typeForm typeForm; //Only for form obj
	int MouseOver; // 1 = Mouse is over, 0 = Not over..
	
} t_object;

typedef struct {
	
	SDL_Color couleur;
	char * content;
	int x;
	int y;
	
} t_text;

typedef struct {

	char * file;
	int x;
	int y;
	int height;
	int width;

} t_texture;

typedef struct {

	char * title;
	SDL_Surface * windowSurface;
	
	t_object * windowObj;
	int nbObj;
	
	t_text * windowText;
	int nbText;
	
	t_texture * windowImg;
	int nbImg;
	
	int x, y;
	int height, width;
	
} t_window;

typedef struct
{
	char key[SDLK_LAST];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
    char quit;
    
} Input;

void SDL_init(int x, int y, char titre[100], int ttf_support, char police_name[100], int police_size, int audio_support);

int SDL_generateMenu(int nb_entree, char sommaire[N][M]);

void SDL_Print_Btn(int id, char titre[30], int x, int y);

void SDL_Blit_Picture(char file[50], int x, int y, int FillRect);

int SDL_IsMouseOver(t_window * window, int hauteur, int largeur, int x, int y);

void SDL_Ambiance(char musicfic[100]);

void SDL_Splash(char img[100], int attente);

int SDL_IsMouseOverObj(t_window * window);
void SDL_BlitObjs(t_window * window);
void SDL_UpdateEvents(Input* in);
int SDL_MessageBox();
int SDL_CaptureForm(t_window * window, int obj);

void SDL_newObj(t_window * window, int * id, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width);
void SDL_modObj(t_window * window, int obj, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width);
void SDL_delObj(t_window * window, int obj);

void SDL_newText(t_window * window, int * id, char * content, SDL_Color couleur, int x, int y);
void SDL_modText(t_window * window, int idtext, char * content, SDL_Color couleur, int x, int y);
void SDL_delText(t_window * window, int idtext);

void SDL_newTexture(t_window * window, int * id, char * file, int x, int y, int height, int width);
void SDL_modTexture(t_window * window, int idimg, char * file, int x, int y, int height, int width);
void SDL_delTexture(t_window * window, int idimg);

int SDL_generate(t_window * window);

t_window * SDL_newWindow(char * title, int x, int y, int height, int width);
void SDL_freeWindow(t_window * window);

/** \brief Calque principal (sur lequel tout est superposé) */
extern SDL_Surface *screen;

/** \brief Couleurs RGB */
extern SDL_Color couleurRouge;
/** \brief Couleurs RGB */
extern SDL_Color couleurBlanche;
/** \brief Couleurs RGB */
extern SDL_Color couleurNoire;

/** \brief Contient la suite des événements capturés par la SDL (Clavier+Souris) */
extern SDL_Event GlobalEvent;
/* Fin pointeur SDL */

/* Variable pour le mixage */
extern int channel;	
/** \brief Mixage du son */
extern Mix_Chunk *sound;

extern Mix_Chunk *music;
/** \brief Mixage du son */
extern int channel_music;

extern int channel_effect;	
/** \brief Mixage du son */
extern Mix_Chunk *effect;

//SDL_mutex *MutexEcran = NULL; Inutile pour l'instant.. Faut s'en passer.. :D
//extern FILE * fichier_db;
/** \brief Identifiant du bouton survolé */
extern int sel_menu_m; //Indice du choix (menu) survolé à un moment t.

extern char buffer; //Where we will keep last char from keyboard !
extern int buffer_deliver;

extern Input in, tmp;