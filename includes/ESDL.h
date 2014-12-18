/**
 * \file ESDL.h
 * \brief EasySDL header
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.1
 * \date 08/12/2014
 *
 * Lib for creating SDL program easily.
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

GPU_Image *BTNOVER = NULL, *BTNNOTOVER = NULL, *FORM = NULL;
Mix_Chunk *SELECT = NULL, *ENTER = NULL;

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
	GPU_Image * GPU_buffer_title;
	GPU_Image * GPU_buffer_content;
	
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
	GPU_Image * GPU_buffer;
	
} t_text;

/**
 * \struct t_texture
 * \brief Keep info about one image
 *
 * t_texture contain info of one image, his size in pixels and his pos
 */
typedef struct {

	char * file;
	int x;
	int y;
	int height;
	int width;
	GPU_Image * GPU_buffer;

} t_texture;

/**
 * \struct t_window
 * \brief This is where all data of previous struct are stored for one window
 *
 * t_window contain all obj, text and textures infos. Contain also window pos and size in pixels.
 */
typedef struct {

	char * title;
	GPU_Image * windowSnap;
	
	t_object * windowObj;
	int nbObj;
	
	t_text * windowText;
	int nbText;
	
	t_texture * windowImg;
	int nbImg;
	
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
	char key[256];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
    char quit;
    
} Input;

/**
 * \fn SDL_init(int x, int y, char titre[100], int ttf_support, char police_name[100], int police_size, int audio_support)
 * \brief Init SDL with your preferences
 *
 * \param x Size in pixel
 * \param y Size in pixel
 * \param titre Title of the SDL handle
 * \param ttf_support Decide if SDL_ttf will load up
 * \param police_name Police filename stored in ressources/ttf/
 * \param police_size Police size in pt
 * \param audio_support Decide if SDL_mixer will load up
 * \return No return value, if it fail, it'll close the program with err message.
 */
void SDL_init(int x, int y, char titre[100], int ttf_support, char police_name[100], int police_size, int audio_support);

/**
 * \fn int SDL_generateMenu(int nb_entree, char sommaire[N][M])
 * \brief Generate menu with button(s)
 *
 * \param nb_entree Number of choise to display
 * \param sommaire[N][M] Contain text caption
 * \return Return the value of choise (0 - (nb_entree-1))
 */
int SDL_generateMenu(int nb_entree, char sommaire[N][M]);
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

void SDL_playwav(char * wavfile, int waitEnd, int *channel);

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

int SDL_nbObj(t_window * window);
int SDL_nbText(t_window * window);
int SDL_nbTexture(t_window * window);
int SDL_windowEmpty(t_window * window);

t_window * SDL_newWindow(char * title, int x, int y, int height, int width);
void SDL_freeWindow(t_window * window);

GPU_Image* SDL_convertTexture(SDL_Surface* surface);
void SDL_loadWindow(t_window * window);
void SDL_loadRessources();

/** \brief Calque principal (sur lequel tout est superposé) */
//extern SDL_Surface *screen;
extern GPU_Target* screen;

/** \brief Couleurs RGB */
extern SDL_Color colorRed;
/** \brief Couleurs RGB */
extern SDL_Color colorWhite;
/** \brief Couleurs RGB */
extern SDL_Color colorBlack;

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