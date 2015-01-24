/**
 * \file ESDL.h
 * \brief EasySDL header
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.5.0
 * \date 23-01-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 *
 */

#ifdef _WIN32
	//windows code goes here
    #include <SDL/SDL.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_image.h>
	#include <fmodex/fmod.h>
#else
	//linux/Unix code goes here
    #include <SDL.h>
	#include <SDL_ttf.h>
	#include <SDL_image.h>
	#include <fmodex/fmod.h>
#endif

#define ALIGN_CENTER 1
#define ALIGN_LEFT 2
#define ALIGN_RIGHT 3

typedef enum {
	
	NUMERIC, // 0-9
	ALPHA, // A-Z
	ALPHANUMERIC, // A-Z or 0-9
	NOMASK, // LATIN-1
	NONE //..
	
} t_typeForm;

typedef enum {

	BUTTON,
	INPUT,
	IMG,
	SPRITE,
	TEXT
	
} t_typeData;

/**
 * \struct t_object
 * \brief Décrit de quoi est fait un objet
 *
 * t_object décrit un bouton ou un champs de saisie
 */

typedef struct {
	
	t_typeData type;
	int x;
	int y;
	char title[50];
	int align;
	char * dest; //Only for form obj
	t_typeForm typeForm; //Only for form obj
	int MouseOver; // 1 = Mouse is over, 0 = Not over..
	SDL_Surface *buffer_title, *buffer_content;
	
} t_object;

/**
 * \struct t_text
 * \brief Contient une ligne de texte pour un rendu SDL
 *
 * t_text décrit une chaine de caractère à transformer en SDL_Surface, ne supporte pas les caractères \x !
 */
typedef struct {
	
	SDL_Color couleur;
	char * content;
	int x;
	int y;
	SDL_Surface *buffer;
	
} t_text;

/**
 * \struct t_image
 * \brief Une image et ses paramètres
 *
 * t_image contient la SDL_Surface de une image ainsi que ses paramètres pour le rendu.
 */
typedef struct {
	
	int x;
	int y;
	SDL_Surface *buffer;

} t_image;

/**
 * \struct t_audio
 * \brief Un son avec le nom fichier associé
 *
 * t_audio contient un son chargé et le nom de fichier associé à celui-ci
 */
typedef struct {
	
	char * file;
	FMOD_SOUND * buffer;
	
} t_audio;

/**
 * \struct t_sprite
 * \brief Une image sprite et ses paramètres
 *
 * t_sprite contient les paramètres utiles à la mise en place d'un sprite (ex: personnage animé).
 */
typedef struct {

	SDL_Surface *buffer;
	SDL_Color transparancy;
	int sp_height, sp_width;
	int x, y;
	int position, animation;
	int hide;

} t_sprite;

/**
 * \struct t_context
 * \brief Représentation d'un contexte et son rendu.
 *
 * t_context contient un ensemble de donnée pour la formation d'une frame dans un état donnée.
 */
typedef struct {

	char * title;
	SDL_Surface * contextSurface;
	
	t_object * contextObj;
	int nbObj;
	
	t_text * contextText;
	int nbText;
	
	t_image * contextImg;
	int nbImg;
	
	t_sprite * contextSprite;
	int nbSprite;
	
	int x, y;
	int height, width;
	
} t_context;

/**
 * \struct Input
 * \brief Contient le mapping des évènements liés au clavier et la souris.
 *
 * L'état des touches / souris à un instant t.
 */
typedef struct
{
	
	char key[SDLK_LAST];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
    char quit;
    
} Input;

void SDL_initWindow(int x, int y, int fullscreen, char * titre, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support);
void SDL_unload();

int SDL_generateMenu(char * backgroundPic, int nb_entree, char ** captions);
int SDL_IsMouseOver(t_context * context, int hauteur, int largeur, int x, int y);

/**
* \fn int SDL_ismouseover(t_context * context, t_typeData type)
* \brief Renvoie l'identifiant d'un objet (bouton, champ de saisie, image, sprite, texte) sur lequel la souris survol le-dit objet.
*
* \param context Le contexte pour lequel il faut vérifier
* \param type Le type à vérifier
* \return Identifiant de l'objet ou -1 si rien.
*/
int SDL_ismouseover(t_context * context, t_typeData type);

/**
* \fn void SDL_generateFrame(t_context * context)
* \brief Construit une frame pour un contexte donnée et la stocke dans la surface principale en attente de rafraichissement.
*
* \param context Le contexte pour lequel il faut construire une frame
* \return void
*/
void SDL_generateFrame(t_context * context);

/**
* \fn void SDL_UpdateEvents(Input* in)
* \brief Met à jour la table des évènements à un instant t.
*
* \param in Tableau dans lequel les données seront inscrite.
* \return void
*/
void SDL_UpdateEvents(Input* in);

int SDL_captureforInput(t_context * context, int obj);

int SDL_newObj(t_context * context, int * id, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y);
int SDL_modObj(t_context * context, int obj, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y);
int SDL_delObj(t_context * context, int obj);

int SDL_newText(t_context * context, int * id, char * content, SDL_Color couleur, int x, int y);
int SDL_modText(t_context * context, int idtext, char * content, SDL_Color couleur, int x, int y);
int SDL_delText(t_context * context, int idtext);

int SDL_newTexture(t_context * context, int * id, char * file, int x, int y);
int SDL_modTexture(t_context * context, int idimg, int x, int y);
int SDL_delTexture(t_context * context, int idimg);

int SDL_newSprite(t_context *context, char * filename, SDL_Color transparancy, int sp_height, int sp_width, int x, int y, int position, int animation, int hide);
int SDL_modSprite(t_context *context, int idSprite, int x, int y, int position, int animation, int hide);
int SDL_delSprite(t_context *context, int idSprite);

/**
* \fn int SDL_generate(t_context * context)
* \brief Génère une frame et l'affiche à l'écran, fonction blocante s'il existe des objets (bouton(s) ou champ(s) de saisie).
*
* \param context Le contexte pour lequel il faut générer une frame
* \return L'identifiant du bouton choisis ou rien.
*/
int SDL_generate(t_context * context);

/**
* \fn int SDL_nbObj(t_context * context)
* \brief Compte le nombre d'objet chargée en mémoire pour un contexte donnée (Comprend les bouttons et les champs de saisie)
*
* \param context Le contexte à vérifier, au préalable chargé avec SDL_newContext
* \return Nombre d'objet chargée
*/
int SDL_nbObj(t_context * context);
/**
* \fn int SDL_nbText(t_context * context)
* \brief Compte le nombre d'objet texte chargée en mémoire pour un contexte donnée
*
* \param context Le contexte à vérifier, au préalable chargé avec SDL_newContext
* \return Nombre d'objet texte chargée
*/
int SDL_nbText(t_context * context);
/**
* \fn int SDL_nbImage(t_context * context)
* \brief Compte le nombre d'image chargée en mémoire pour un contexte donnée
*
* \param context Le contexte à vérifier, au préalable chargé avec SDL_newContext
* \return Nombre d'image chargée
*/
int SDL_nbImage(t_context * context);

/**
* \fn int SDL_nbSprite(t_context * context)
* \brief Compte le nombre de sprite chargée en mémoire pour un contexte donnée
*
* \param context Le contexte à vérifier, au préalable chargé avec SDL_newContext
* \return Nombre de sprite chargée
*/
int SDL_nbSprite(t_context * context);

/**
* \fn int SDL_contextEmpty(t_context * context)
* \brief Vérifie si un contexte n'a pas de donnée chargé en mémoire
*
* \param context Le contexte à vérifier, au préalable chargé avec SDL_newContext
* \return Bool
*/
int SDL_contextEmpty(t_context * context);

/**
* \fn t_context * SDL_newContext(char * title, int x, int y, int height, int width)
* \brief Création d'un nouveau contexte
*
* \param title Le titre associé à ce nouveau contexte
* \param x Position x relative à la fenêtre SDL
* \param y Position y relative à la fenêtre SDL
* \param height Hauteur occupé en pixel par ce nouveau contexte
* \param width Largeur occupé en pixel par ce nouveau contexte
* \return Adresse du nouvelle element t_context *
*/
t_context * SDL_newContext(char * title, int x, int y, int height, int width);

/**
* \fn void SDL_freeContext(t_context * context)
* \brief Libère la mémoire, supprime les données chargés pour un contexte donnée.
*
* \param key Identifiant de la touche (constante)
* \return Bool
*/
void SDL_freeContext(t_context * context);

void SDL_loadRessources();
void SDL_unloadRessources();

/**
* \fn int SDL_isKeyPressed(int key)
* \brief Vérifie si une touche du clavier est enfoncée.
*
* \param key Identifiant de la touche (constante)
* \return Bool
*/
int SDL_isKeyPressed(int key);

/**
* \fn int SDL_playSound(char * sndfile)
* \brief Lecture d'un fichier son court au préalable chargé avec SDL_loadSound
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_playSound(char * sndfile);
/**
* \fn int SDL_loadSound(char * sndfile)
* \brief Chargement en mémoire d'un fichier audio
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_loadSound(char * sndfile);
/**
* \fn int SDL_unloadSound(char * sndfile)
* \brief Libérer un fichier audio de la mémoire au préalable chargé avec SDL_loadSound
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_unloadSound(char * sndfile);
/**
* \fn int SDL_unloadallSound()
* \brief Libère tout les fichiers audio chargé avec SDL_loadSound
*
* \return Bool
*/
int SDL_unloadallSound();

extern SDL_Surface *BTN_NOTOVER, *BTN_OVER, *FORM;
extern FMOD_SOUND *SELECT, *ENTER;

t_audio * fmodbuffer;
extern int nbSnd;

extern SDL_Surface *screen;
extern SDL_Color colorRed, colorWhite, colorBlack, colorGreenLight;
/** \brief Contient la suite des événements capturés par la SDL (Clavier+Souris) */
extern SDL_Event GlobalEvent;

/** \brief Identifiant du bouton survolé */
extern char buffer; //Where we will keep last char from keyboard !
extern int buffer_deliver;
extern Input in;
