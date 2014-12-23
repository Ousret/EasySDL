/**
 * \file ESDL.h
 * \brief EasySDL header
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.4.5
 * \date 22/12/2014
 *
 * Lib for creating SDL program easily.
 *
 */

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
 
#define N 10
#define M 50

typedef enum {
	
	NUMERIC,
	VARCHAR,
	NUMCHAR,
	ALL
	
} t_typeForm;

/**
 * \struct t_object
 * \brief Represent object, could be button or form input to be filled.
 *
 * t_object contain info of one object (button or form input)
 */

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
	SDL_Surface *buffer_title, *buffer_content;
	
} t_object;

/**
 * \struct t_text
 * \brief Represent text only with color and x, y pos (in pixel)
 *
 * t_text contain info of one text line does not support char like \n, \t, etc.. .
 */
typedef struct {
	
	SDL_Color couleur;
	char * content;
	int x;
	int y;
	SDL_Surface *buffer;
	
} t_text;

/**
 * \struct t_texture
 * \brief Keep info about one image
 *
 * t_texture contain info of one image, his size in pixels and his pos
 */
typedef struct {
	
	int x;
	int y;
	int height;
	int width;
	SDL_Surface *buffer;

} t_texture;

typedef struct {
	
	char file[150];
	Mix_Chunk * MIX_BUF;
	
} t_audio;

typedef struct {

	SDL_Surface *buffer;
	SDL_Color transparancy;
	int height, width;
	int sp_height, sp_width;
	int nb_elem;
	int x, y;
	int current;

} t_sprite;

/**
 * \struct t_window
 * \brief This is where all data of previous struct are stored for one window
 *
 * t_window contain all obj, text and textures infos. Contain also window pos and size in pixels.
 */
typedef struct {

	char * title;
	SDL_Surface * windowSurface;
	
	t_object * windowObj;
	int nbObj;
	
	t_text * windowText;
	int nbText;
	
	t_texture * windowImg;
	int nbImg;
	
	t_sprite * windowSprite;
	int nbSprite;
	
	int x, y;
	int height, width;
	
} t_window;

/**
 * \struct Input
 * \brief This struct help us to keep all event with UpdateEvents
 *
 * Input is like mapping of what being pressed or not at t instant.
 */
typedef struct
{
	char key[SDLK_LAST];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
    char quit;
    
} Input;

/**
 * \fn SDL_init(int x, int y, int fullscreen, char titre[100], int ttf_support, char police_name[100], int police_size, int audio_support)
 * \brief Init SDL with your preferences
 *
 * \param x Size in pixel
 * \param y Size in pixel
 * \param fullscreen Enable fullscreen or not
 * \param titre Title of the SDL handle
 * \param ttf_support Decide if SDL_ttf will load up
 * \param police_name Police filename stored in ressources/ttf/
 * \param police_size Police size in pt
 * \param audio_support Decide if SDL_mixer will load up
 * \return No return value, if it fail, it'll close the program with err message.
 */
void SDL_init(int x, int y, int fullscreen, char * titre, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support);

/**
 * \fn int SDL_generateMenu(int nb_entree, char sommaire[N][M])
 * \brief Generate menu with button(s)
 *
 * \param nb_entree Number of choise to display
 * \param sommaire[N][M] Contain text caption
 * \return Return the value of choise (0 - (nb_entree-1))
 */
int SDL_generateMenu(int nb_entree, char captions[][M]);

/**
 * \fn int SDL_IsMouseOver(t_window * window, int hauteur, int largeur, int x, int y)
 * \brief Check if mouse is over area definied in pixel
 *
 * \param *window Ptr to window created with SDL_newwindow
 * \param hauteur Height in pixel
 * \param largeur Width in pixel
 * \param x Pos X in pixel
 * \param y Pos Y in pixel 
 * \return Bool func
 */
int SDL_IsMouseOver(t_window * window, int hauteur, int largeur, int x, int y);

/**
 * \fn void SDL_playwav(char * wavfile, int waitEnd, int *channel);
 * \brief Play wav file
 *
 * \param wavfile Name of the wav file to play (must be in ressources/snd/)
 * \param waitEnd Stop the current thread until playing is over
 * \param channel Channel used by SDL_Mixer to play WAV. (PTR)
 * \return void
 */
void SDL_playwav(char * wavfile, int waitEnd, int *channel);

void SDL_Splash(char img[100], int attente);

/**
 * \fn int SDL_IsMouseOverObj(t_window * window);
 * \brief Check if mouse is over an object and return his obj id
 *
 * \param window Window name (created with newWindow())
 * \return Obj id or -1
 */
int SDL_IsMouseOverObj(t_window * window);

/**
 * \fn void SDL_BlitObjs(t_window * window);
 * \brief Print on the screen specific window data
 *
 * \param window Window name (created with newWindow())
 * \return void
 */
void SDL_BlitObjs(t_window * window);

/**
 * \fn void SDL_UpdateEvents(Input* in);
 * \brief Get at t instant event from input
 *
 * \param in Input struct that contains key mapping with 0/1 for pressed or not
 * \return void
 */
void SDL_UpdateEvents(Input* in);

/**
 * \fn int SDL_CaptureForm(t_window * window, int obj);
 * \brief Copy buffer (key.unicode) to form obj if mouse is over his area
 *
 * \param window Window that contain form obj
 * \param obj ObjID (form) dest for buffer cpy
 * \return void
 */
int SDL_CaptureForm(t_window * window, int obj);

/**
 * \fn void SDL_newObj(t_window * window, int * id, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width);
 * \brief Create a new object into *window data buffer
 *
 * \param window Window target
 * \param id Get the ID of this new obj if created
 * \param type 0 = Button, 1 = Form to fill
 * \param title Caption of this new object
 * \param dest Addr of char * var if type = 1
 * \param typeForm If type=1, you need to precise the mask
 * \param x Pos X, relative to window
 * \param y Pos Y, relative to window
 * \param height Height in pixel
 * \param width Width in pixel
 * \return void
 */
int SDL_newObj(t_window * window, int * id, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width);
int SDL_modObj(t_window * window, int obj, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width);
int SDL_delObj(t_window * window, int obj);

int SDL_newText(t_window * window, int * id, char * content, SDL_Color couleur, int x, int y);
int SDL_modText(t_window * window, int idtext, char * content, SDL_Color couleur, int x, int y);
int SDL_delText(t_window * window, int idtext);

int SDL_newTexture(t_window * window, int * id, char * file, int x, int y, int height, int width);
int SDL_modTexture(t_window * window, int idimg, char * file, int x, int y, int height, int width);
int SDL_delTexture(t_window * window, int idimg);

int SDL_newSprite(t_window *window, char * filename, SDL_Color transparancy, int height, int width,int sp_height, int sp_width, int x, int y, int nb_elem, int current);
int SDL_modSprite(t_window *window, int idSprite, int x, int y, int current);
int SDL_delSprite(t_window *window, int idSprite);

int SDL_generate(t_window * window);

int SDL_nbObj(t_window * window);
int SDL_nbText(t_window * window);
int SDL_nbTexture(t_window * window);
int SDL_windowEmpty(t_window * window);

t_window * SDL_newWindow(char * title, int x, int y, int height, int width);

void SDL_freeWindow(t_window * window);

void SDL_loadRessources();
void SDL_unload();
int SDL_isKeyPressed(int key);
void SDL_unloadRessources();
void SDL_preloadwav(char * wavfile);

extern SDL_Surface *BTN_NOTOVER, *BTN_OVER, *FORM;
extern Mix_Chunk *SELECT, *ENTER;

t_audio MIXTEMP[50];
extern int nbSnd;

extern SDL_Surface *screen;

/** \brief Couleurs RGB */
extern SDL_Color colorRed;
/** \brief Couleurs RGB */
extern SDL_Color colorWhite;
/** \brief Couleurs RGB */
extern SDL_Color colorBlack;

/** \brief Contient la suite des événements capturés par la SDL (Clavier+Souris) */
extern SDL_Event GlobalEvent;
/* Fin pointeur SDL */

//SDL_mutex *MutexEcran = NULL; Inutile pour l'instant.. Faut s'en passer.. :D
//extern FILE * fichier_db;
/** \brief Identifiant du bouton survolé */
extern int sel_menu_m; //Indice du choix (menu) survolé à un moment t.

extern char buffer; //Where we will keep last char from keyboard !

extern int buffer_deliver;

extern Input in, tmp;