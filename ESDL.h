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

typedef enum {

	ALIGN_CENTER = 1, /*!< Texte aligné au centre */
	ALIGN_LEFT, /*!< Texte aligné à gauche */
	ALIGN_RIGHT /*!< Texte aligné à droite */
	
} t_typeAlign;

typedef enum {
	
	NUMERIC, /*!< Capture uniquement les chiffres de 0 à 9 */
	ALPHA, /*!< Capture uniquement les caractères de A à Z */
	ALPHANUMERIC, /*!< Capture uniquement les caractères alphanumériques */
	NOMASK, /*!< Capture tout, aucun masque de saisie */
	NONE /*!< Lorsqu'il ne s'agit pas d'un champ de saisie */
	
} t_typeForm;

typedef enum {

	BUTTON, /*!< Les boutons interactifs */
	INPUT, /*!< Les champs de saisie */
	IMG, /*!< Les images */
	SPRITE, /*!< Les sprites */
	TEXT /*!< Le texte */
	
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
 * t_text décrit une chaine de caractère à transformer en SDL_Surface, ne supporte pas les caractères de type '\\x' !
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
	
	int x; /*!< Position x relative au contexte parent */
	int y; /*!< Position y relative au contexte parent */
	SDL_Surface *buffer; /*!< La surface SDL de l'image */

} t_image;

/**
 * \struct t_audio
 * \brief Un son avec le nom fichier associé
 *
 * t_audio contient un son chargé et le nom de fichier associé à celui-ci
 */
typedef struct {
	
	char * file; /*!< Nom du fichier audio associé */
	FMOD_SOUND * buffer; /*!< Buffer du fichier audio chargé */
	
} t_audio;

/**
 * \struct t_sprite
 * \brief Une image sprite et ses paramètres
 *
 * t_sprite contient les paramètres utiles à la mise en place d'un sprite (ex: personnage animé).
 */
typedef struct {

	SDL_Surface *buffer; /*!< Buffer de l'image sprite utile */
	SDL_Color transparancy; /*!< Couleur clé pour la transparence */
	int sp_height, sp_width; 
	int x, y;
	int position, animation;
	int hide; /*!< Indique si le sprite doit être afficher */

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

/**
* \fn void SDL_initWindow(int width, int height, int fullscreen, char * title, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support)
* \brief Initialise les modules de base et ouvre une fenêtre SDL
*
* \param width Largeur de la fenêtre SDL
* \param height Hauteur de la fenêtre SDL
* \param fullscreen Plein écran = 1, fenêtre = 0.
* \param title Titre de la fenêtre SDL
* \param icon_name Chemin vers l'icone de l'application sinon NULL pour aucune icone
* \param ttf_support Charge le module de rendu texte = 1 sinon 0
* \param police_name Nom du fichier *.ttf pour le moteur de rendu texte
* \param police_size Taille en pt de la police d'écriture
* \param audio_support Prend en charge l'audio avec fmodex = 1 sinon 0
* \return void
*/
void SDL_initWindow(int width, int height, int fullscreen, char * title, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support);

/**
* \fn void SDL_unload()
* \brief Décharge la mémoire des modules précédemment chargés et ferme le processus
*
* \return void
*/
void SDL_unload();

/**
* \fn int SDL_generateMenu(char * backgroundPic, int nbEntries, char ** captions)
* \brief Génère un menu générique et attends que l'utilisateur fasse un choix
*
* \param backgroundPic Nom de l'image à utiliser en fond
* \param nbEntries Nombre de bouton à générer
* \param captions Contient le texte associés au(x) bouton(s) (matrice **char)
* \return Identifiant du bouton choisis
*/
int SDL_generateMenu(char * backgroundPic, int nbEntries, char ** captions);

/**
* \fn int SDL_IsMouseOver(t_context * context, int hauteur, int largeur, int x, int y)
* \brief Vérifie si la souris est dans la zone définie en paramètre
*
* \param context Contexte de référence concerné
* \param hauteur Hauteur de la zone en pixel
* \param largeur Largeur de la zone en pixel
* \param x Coordonnée x du point supérieur gauche
* \param y Coordonnée y du point supérieur gauche
* \return bool
*/
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

/**
* \fn int SDL_captureforInput(t_context * context, int obj)
* \brief Copie le dernier caractère capturé dans la bonne destination
*
* \param context Contexte concerné
* \param obj Identifiant de l'objet concerné
* \return bool
*/
int SDL_captureforInput(t_context * context, int obj);

/**
* \fn int SDL_newObj(t_context * context, int * id, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y)
* \brief Création d'un nouvelle objet (bouton ou champs de saisie)
*
* \param context Contexte concerné
* \param id Retourne l'identifiant de l'objet crée mettre à NULL si vous ne souhaitez pas récupérer son identifiant.
* \param type Bouton ou champ de saisie
* \param title Titre de l'objet
* \param align Alignement du texte
* \param dest S'il s'agit d'un champs de saisie, mettre la variable string dans laquel sera stocké le résultat, sinon mettre à NULL.
* \param typeForm S'il s'agit d'un champs de saisie, préciser le masque de saisie, sinon mettre NONE.
* \param x Position x relative au contexte de l'objet
* \param y Position y relative au contexte de l'objet
* \return bool
*/
int SDL_newObj(t_context * context, int * id, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y);
/**
* \fn int SDL_modObj(t_context * context, int obj, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y)
* \brief Modifie les propriétés d'un objet
*
* \param context Contexte concerné
* \param obj Identifiant de l'objet à modifier
* \param type Bouton ou champ de saisie
* \param title Titre de l'objet
* \param align Alignement du texte
* \param dest S'il s'agit d'un champs de saisie, mettre la variable string dans laquel sera stocké le résultat, sinon mettre à NULL.
* \param typeForm S'il s'agit d'un champs de saisie, préciser le masque de saisie, sinon mettre NONE.
* \param x Position x relative au contexte de l'objet
* \param y Position y relative au contexte de l'objet
* \return bool
*/
int SDL_modObj(t_context * context, int obj, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y);
/**
* \fn int SDL_delObj(t_context * context, int obj)
* \brief Supprime d'un contexte un objet
*
* \param context Contexte concerné
* \param obj Identifiant de l'objet à supprimer
* \return bool
*/
int SDL_delObj(t_context * context, int obj);

/**
* \fn int SDL_newText(t_context * context, int * id, char * content, SDL_Color couleur, int x, int y)
* \brief Créer une ligne de texte à afficher pour un contexte 
*
* \param context Contexte concerné
* \param id Retourne l'identifiant de l'objet texte crée mettre à NULL si vous ne souhaitez pas récupérer son identifiant.
* \param content La chaine de caractère à associé à cette objet texte.
* \param couleur La couleur du texte
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_newText(t_context * context, int * id, char * content, SDL_Color couleur, int x, int y);
/**
* \fn int SDL_modText(t_context * context, int idtext, char * content, SDL_Color couleur, int x, int y)
* \brief Modifie une entrée texte au préalable chargée avec SDL_newText()
*
* \param context Contexte concerné
* \param idtext Identifiant de l'objet texte à modifier
* \param content La chaine de caractère à associé à cette objet texte.
* \param couleur La couleur du texte
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_modText(t_context * context, int idtext, char * content, SDL_Color couleur, int x, int y);
/**
* \fn int SDL_delText(t_context * context, int idtext)
* \brief Supprime d'un contexte un objet texte
*
* \param context Contexte concerné
* \param idtext Identifiant de l'objet texte à supprimer
* \return bool
*/
int SDL_delText(t_context * context, int idtext);

/**
* \fn int SDL_newImage(t_context * context, int * id, char * file, int x, int y)
* \brief Ajoute une image à un contexte
*
* \param context Contexte concerné
* \param id Retourne l'identifiant de l'image crée mettre à NULL si vous ne souhaitez pas récupérer son identifiant.
* \param file Le fichier image concerné
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_newImage(t_context * context, int * id, char * file, int x, int y);
/**
* \fn int SDL_modImage(t_context * context, int idimg, int x, int y)
* \brief Modifie les propriétés d'une image
*
* \param context Contexte concerné
* \param idimg Identifiant de l'image dont les propriétés sont à modifier
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_modImage(t_context * context, int idimg, int x, int y);
/**
* \fn int SDL_delImage(t_context * context, int idimg)
* \brief Supprimer une image d'un contexte
*
* \param context Contexte concerné
* \param idimg Identifiant de l'image à supprimer
* \return bool
*/
int SDL_delImage(t_context * context, int idimg);

/**
* \fn int SDL_newSprite(t_context *context, char * filename, SDL_Color transparancy, int sp_height, int sp_width, int x, int y, int position, int animation, int hide)
* \brief Ajoute un sprite pour un contexte
*
* \param context Contexte concerné
* \param filename Fichier image du sprite (transparence png recommandé)
* \param transparancy Couleur de transparence 
* \param sp_height Hauteur d'une partie (souvent le personnage)
* \param sp_width Largeur d'une partie (souvent le personnage)
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \param position Identifiant de la colonne (verticale)
* \param animation Identifiant de la ligne (horizontale)
* \param hide Pour ne pas afficher = 1, 0 pour afficher.
* \return bool
*/
int SDL_newSprite(t_context *context, char * filename, SDL_Color transparancy, int sp_height, int sp_width, int x, int y, int position, int animation, int hide);
/**
* \fn int SDL_modSprite(t_context *context, int idSprite, int x, int y, int position, int animation, int hide)
* \brief Modifie le sprite d'un contexte donnée à condition qu'il soit déjà chargé avec SDL_newSprite()
*
* \param context Contexte concerné
* \param idSprite Identifiant du sprite concerné
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \param position Identifiant de la colonne (verticale)
* \param animation Identifiant de la ligne (horizontale)
* \param hide Pour ne pas afficher = 1, 0 pour afficher.
* \return bool
*/
int SDL_modSprite(t_context *context, int idSprite, int x, int y, int position, int animation, int hide);
/**
* \fn int SDL_delSprite(t_context *context, int idSprite)
* \brief Supprime un sprite d'un contexte donnée
*
* \param context Contexte concerné
* \param idSprite Identifiant du sprite à supprimer
* \return bool
*/
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
* \param context Contexte concerné
* \return void
*/
void SDL_freeContext(t_context * context);

/**
* \fn void SDL_loadRessources()
* \brief Charge en mémoire les ressources de base (Image bouton, champ saisie, son de selection, etc..)
*
* \return void
*/
void SDL_loadRessources();
/**
* \fn void SDL_unloadRessources()
* \brief Libère la mémoire des ressources de base, précédemment chargées avec SDL_loadRessources()
*
* \return void
*/
void SDL_unloadRessources();

/**
* \fn int SDL_isKeyPressed(int KEY_S)
* \brief Vérifie si une touche du clavier est enfoncée.
*
* \param KEY_S Identifiant de la touche (constante SDL_Keycode) (https://wiki.libsdl.org/SDL_Keycode)
* \return Bool
*/
int SDL_isKeyPressed(int KEY_S);

/**
* \fn int SDL_isMousePressed(int MOUSE_S)
* \brief Vérifie si une touche de la souris est enfoncée.
*
* \param MOUSE_S Identifiant de la touche (constante SDL_Keycode) (https://wiki.libsdl.org/SDL_Keycode)
* \return Bool
*/
int SDL_isMousePressed(int MOUSE_S);

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

/**
* \fn void SDL_setDelaySingleFrame(int delay)
* \brief Le temps de rafraichissement entre chaque frame, par défault 50 ms.
*
* \param delay Temps en milliseconde entre chaque frame rendu
* \return void
*/
void SDL_setDelaySingleFrame(int delay);

/**
* \fn int SDL_requestExit()
* \brief Vérifie si l'utilisateur requiert la fermeture du programme
*
* \return bool
*/
int SDL_requestExit();

extern SDL_Surface *BTN_NOTOVER, *BTN_OVER, *FORM;
extern FMOD_SOUND *SELECT, *ENTER;

t_audio * fmodbuffer;
extern int nbSnd;

extern int DELAY_EACH_FRAME;

extern SDL_Surface *screen;
extern SDL_Color colorRed, colorWhite, colorBlack, colorGreenLight;
/** \brief Contient la suite des événements capturés par la SDL (Clavier+Souris) */
extern SDL_Event GlobalEvent;

/** \brief Identifiant du bouton survolé */
extern char buffer; //Where we will keep last char from keyboard !
extern int buffer_deliver;
extern Input in;
