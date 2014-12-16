/** EasySDL Alpha
*
*	Author(s): TAHRI Ahmed, SIMON J�r�my
*	Lib: EasySDL
*	Version: 0.1
* 	Date: 8-12-2014
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
/* NOT USING IT FOR NOW 
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
*/
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string.h>

#include "includes/ESDL.h"

SDL_Surface *screen = NULL;
SDL_Surface *BTN_NOTOVER = NULL, *BTN_OVER = NULL, *FORM = NULL;
Mix_Chunk *SELECT = NULL, *ENTER = NULL;

SDL_Color colorRed = {255, 0, 0};
SDL_Color colorWhite = {255, 255, 255};
SDL_Color colorBlack = {0, 0, 0};

SDL_Event GlobalEvent;

char buffer = 0;
int buffer_deliver = 1;

TTF_Font *ttf_police = NULL;	

int channel = 0, channel_effect = 0, channel_music = 0;
Mix_Chunk *sound = NULL, *effect = NULL, *music = NULL;

int sel_menu_m = 0;
int tff_loaded = 0, audio_loaded = 0;

Input in;

void SDL_playwav(char * wavfile, int waitEnd, int *channel) {
	
	char filePath[150];
	int newChannel = 0;
	memset(filePath, 0, sizeof(filePath));
	
	sprintf(filePath, "ressources/snd/%s", wavfile);
	
	Mix_Chunk *sound = Mix_LoadWAV(filePath);
	newChannel = Mix_PlayChannel(-1, sound, 0);
	
	if (channel != NULL) *channel = newChannel;
	if (waitEnd == 1) {
		while(Mix_Playing(newChannel) != 0);
		Mix_FreeChunk(sound);
	}
}

int SDL_nbObj(t_window * window) {
	return (window->nbObj);
}

int SDL_nbText(t_window * window) {
	return (window->nbText);
}

int SDL_nbTexture(t_window * window) {
	return (window->nbImg);
}

int SDL_windowEmpty(t_window * window) {
	if (window == NULL) return 1;
	if ((SDL_nbObj(window) + SDL_nbText(window) + SDL_nbTexture(window)) == 0) return 1;
	return 0;
}

int SDL_IsMouseOverObj(t_window * window) {
	
	int i = 0, overobj = -1;
	
	for (i = 0; i < (window->nbObj); i++) {
		
		if (SDL_IsMouseOver(window, window->windowObj[i].height, window->windowObj[i].width, window->windowObj[i].x, window->windowObj[i].y)) {
			window->windowObj[i].MouseOver = 1;
			overobj = i;
		}else{
			window->windowObj[i].MouseOver = 0;
		}
		
	}
	
	return overobj; //Nothing here!
	
}

void SDL_init(int width, int height, char title[100], int ttf_support, char police_name[100], int police_size, int audio_support) {
	
    int sdl_start = 0;
	char file[100]; //Generating file path
	
	memset(file, 0, sizeof(file));
	
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	
    /* Initialize SDL */
    if (audio_support == 1) {
    	sdl_start = SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    }else {
    	sdl_start = SDL_Init (SDL_INIT_VIDEO);
    }
    
    if (sdl_start < 0)
    {
        
        fprintf (stderr, "[!] SDL failed to load because of: %s\n", SDL_GetError ());
        exit (1);
        
    }
    
	atexit (SDL_Quit);

    screen = SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // | SDL_FULLSCREEN
    
    if (screen == NULL)
    {
        
        fprintf (stderr, "[!] Unable to load window at %ix%i in 32 bits': %s\n", width, height ,SDL_GetError ());
        exit (2);
        
    }
    
	SDL_WM_SetCaption (title, NULL);
	
	if (audio_support == 1) {
		
		if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
			
			fprintf (stderr, "[!] Cannot load SDL audio because of: %s\n", Mix_GetError());
		    exit (1);
		    
		}
		audio_loaded = 1;
	}
	
	if (ttf_support == 1) {
		
		if(TTF_Init() == -1)
		{
	    	fprintf (stderr, "[!] Cannot load SDL_ttf, you may want to check your SDL setup !\n");
	        exit (1);
		}
		
		sprintf(file, "ressources/ttf/%s", police_name);
		
		/*if (!STJ_FileExist(file)) {
			fprintf(stderr, "[!] Unable to find %s ttf file !\n", police_name);
			exit (1);
		}*/
		
		if (police_size <= 0) {
			fprintf(stderr, "[!] Cannot load ttf_police with size=0 !\n");
			exit (1);
		}
		
		ttf_police = TTF_OpenFont(file, police_size);
		tff_loaded = 1;
	}
	
	SDL_EnableUNICODE(1);
	
	int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	int initted=IMG_Init(flags);
	
	if((initted&flags) != flags) {
    	fprintf(stdout, "[!] IMG_Init: Failed to init required jpg and png support!\n");
    	fprintf(stdout, "[!] IMG_Init: %s\n", IMG_GetError());
    	exit (1);
	}
	
	SDL_loadRessources();
	
}

int SDL_CaptureForm(t_window * window, int obj) {

	int current_len = 0;
	//Return if there's nothing to process
	if (window == NULL) return 0;
	
	if ((obj != -1) && (window->windowObj[obj].dest != NULL) && (buffer_deliver == 0)) {
			
		if ((window->windowObj[obj].type) == 1) {
			
			current_len = strlen(window->windowObj[obj].dest);
			
			if (current_len < 35) {
				
				if (buffer == '\b' || buffer == 127) { //127 pour clavier MacOS X BT
					
					if (current_len > 0) {
							
						window->windowObj[obj].dest[current_len-1] = '\0';
							
					}
					
				}else {
					
					switch (window->windowObj[obj].typeForm) {
					
						case NUMERIC:
							if (buffer >= '0' && buffer <= '9') {
								window->windowObj[obj].dest[current_len] = buffer;
								window->windowObj[obj].dest[current_len+1] = '\0';
							}
							break;
							
						case VARCHAR:
							if ((buffer >= 'a' && buffer <= 'z') || (buffer >= 'A' && buffer <= 'Z')) {
								window->windowObj[obj].dest[current_len] = buffer;
								window->windowObj[obj].dest[current_len+1] = '\0';
							}
							
							break;
							
						case NUMCHAR:
							if ((buffer >= 'a' && buffer <= 'z') || (buffer >= 'A' && buffer <= 'Z') || (buffer >= '0' && buffer <= '9')) {
								window->windowObj[obj].dest[current_len] = buffer;
								window->windowObj[obj].dest[current_len+1] = '\0';
							}
							
							break;
							
						case ALL:
							
							window->windowObj[obj].dest[current_len] = buffer;
							window->windowObj[obj].dest[current_len+1] = '\0';
							break;
							
						default:
						
							window->windowObj[obj].dest[current_len] = buffer;
							window->windowObj[obj].dest[current_len+1] = '\0';
							break;
					
					}
					
					
					
				}
				
			}
			
		buffer_deliver = 1;
		return 1;
			
		}else{
			
			//Wrong dest ! Take your buffer away from us !
			buffer_deliver = 1;
			return 0;
		}
			
			
	}else{
			
		buffer_deliver = 1;
		return 0;	
	}

}

t_window * SDL_newWindow(char * title, int x, int y, int height, int width) {

	t_window * tmp = malloc(sizeof(t_window));
	tmp->title = title;
	
	tmp->windowSurface = NULL;
	
	tmp->windowObj = NULL;
	tmp->windowText = NULL;
	tmp->windowImg = NULL;
	
	tmp->nbObj = 0;
	tmp->nbObj_loaded = 0;
	tmp->nbText = 0;
	tmp->nbText_loaded = 0;
	tmp->nbImg = 0;
	tmp->nbImg_loaded = 0;
	
	tmp->x = x;
	tmp->y = y;
	
	tmp->height = height;
	tmp->width = width;
	
	return tmp;

}

void SDL_freeWindow(t_window * window) {
	
	if (window == NULL) return;
	int i = 0;
	
	if (window->windowObj != NULL) {
		for (i = 0;i < (window->nbObj); i ++) {
			if (window->windowObj[i].buffer_title)
				SDL_FreeSurface(window->windowObj[i].buffer_title);
				window->windowObj[i].buffer_title = NULL;
			if (window->windowObj[i].buffer_content)
				SDL_FreeSurface(window->windowObj[i].buffer_content);
				window->windowObj[i].buffer_content = NULL;
		}
		free (window->windowObj);
	}
	
	if (window->windowText != NULL) {
		for (i = 0;i < (window->nbText); i ++) {
			if (window->windowText[i].buffer)
				SDL_FreeSurface(window->windowText[i].buffer);
				window->windowText[i].buffer = NULL;
		}
		free (window->windowText);
	}
	
	if (window->windowImg != NULL) {
		for (i = 0;i < (window->nbImg); i ++) {
			if (window->windowImg[i].buffer)
				SDL_FreeSurface(window->windowImg[i].buffer);
				window->windowImg[i].buffer = NULL;
		}
		free (window->windowImg);
	}
	
	free (window);
	
}

void SDL_newObj(t_window * window, int * id, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width) {
	
	if (window == NULL) return;
	
	if (window->nbObj == 0) {
		
		if (window->windowObj == NULL) {
			window->windowObj = malloc(sizeof(t_object));
		}else{
			return;
		}
		
	}else{
	
		window->windowObj = realloc(window->windowObj, sizeof(t_object) * ((window->nbObj)+2));
		
	}
	
	window->windowObj[window->nbObj].x = x;
	window->windowObj[window->nbObj].y = y;
	window->windowObj[window->nbObj].height = height;
	window->windowObj[window->nbObj].width = width;
	window->windowObj[window->nbObj].MouseOver = 0;
	window->windowObj[window->nbObj].buffer_title = NULL;
	window->windowObj[window->nbObj].buffer_content = NULL;
	
	strcpy(window->windowObj[window->nbObj].title, title);
	
	window->windowObj[window->nbObj].type = type;
	
	
	if (type == 1) {
		
		window->windowObj[window->nbObj].dest = dest;
		window->windowObj[window->nbObj].typeForm = typeForm;
		
	}else {
		
		window->windowObj[window->nbObj].dest = NULL;
		window->windowObj[window->nbObj].typeForm = ALL;
		
	}
	
	if (id != NULL) *id = window->nbObj;
	
	window->nbObj = (window->nbObj+1);
	
	return;
	
}

void SDL_newTexture(t_window * window, int * id, char * file, int x, int y, int height, int width) {

	if (window == NULL) return;
	
	if (window->nbImg == 0) {
	
		if (window->windowImg == NULL) {
		
			window->windowImg = malloc(sizeof(t_texture));
			
		}else{
			
			return;
			
		}
	
	}else{
	
		window->windowImg = realloc(window->windowImg, sizeof(t_texture) * ((window->nbImg) + 2));
	
	}
	
	window->windowImg[window->nbImg].file = file;
	window->windowImg[window->nbImg].x = x;
	window->windowImg[window->nbImg].y = y;
	window->windowImg[window->nbImg].height = height;
	window->windowImg[window->nbImg].width = width;
	window->windowImg[window->nbImg].buffer = NULL;
	
	if (id != NULL) *id = (window->nbImg);
	
	window->nbImg = (window->nbImg)+1;
	
	return;

}

void SDL_modTexture(t_window * window, int idimg, char * file, int x, int y, int height, int width) {

	if (window == NULL) return;
	if (window->windowImg == NULL) return;
	if (window->nbImg < idimg) return;
	
	window->windowImg[idimg].file = file;
	window->windowImg[idimg].x = x;
	window->windowImg[idimg].y = y;
	window->windowImg[idimg].height = height;
	window->windowImg[idimg].width = width;
	
	return;

}

void SDL_delTexture(t_window * window, int idimg) {

	if (window == NULL) return;
	if (window->windowImg == NULL) return;
	if (window->nbImg < idimg) return;
	
	int i = 0;
	
	for (i = idimg; i < window->nbImg; i++) {
	
		window->windowImg[i] = window->windowImg[i+1];
	
	}
	
	window->nbImg = (window->nbImg)-1;
	
	return;

}

void SDL_modObj(t_window * window, int obj, int type, char title[50], char * dest, t_typeForm typeForm, int x, int y, int height, int width) {

	if (window == NULL) return;
	if (window->nbObj < obj) return;
	if (window->windowObj == NULL) return;
	
	window->windowObj[obj].x = x;
	window->windowObj[obj].y = y;
	window->windowObj[obj].height = height;
	window->windowObj[obj].width = width;
	window->windowObj[obj].MouseOver = 0;
	
	strcpy(window->windowObj[obj].title, title);
	
	window->windowObj[obj].type = type;
	
	
	if (type == 1) {
		
		window->windowObj[obj].dest = dest;
		window->windowObj[obj].typeForm = typeForm;
		
	}else {
		
		window->windowObj[obj].dest = NULL;
		window->windowObj[obj].typeForm = ALL;
		
	}
	
	return;
	
}

void SDL_delObj(t_window * window, int obj) {

	if (window == NULL) return;
	if (window->nbObj < obj) return;
	if (window->windowObj == NULL) return;
	
	int i = 0;
	
	for (i = obj; i < (window->nbObj); i++) {
	
		window->windowObj[i] = window->windowObj[i+1];
	
	}
	
	window->nbObj = (window->nbObj)-1;
	
	return;

}

void SDL_newText(t_window * window, int * id, char * content, SDL_Color couleur, int x, int y) {

	if (window == NULL) return;
	
	if (window->nbText == 0) {
	
		if (window->windowText == NULL) {
			window->windowText = malloc(sizeof(t_text));
		}else{
			return;
		}
	
	}else{
		 window->windowText = realloc(window->windowText, sizeof(t_text) * ((window->nbText) + 2));
	}
	
	window->windowText[window->nbText].couleur = couleur;
	window->windowText[window->nbText].content = content;
	
	window->windowText[window->nbText].x = x;
	window->windowText[window->nbText].y = y;
	
	if (id != NULL) *id = (window->nbText);
	window->nbText = (window->nbText)+1;
	
	return;

}

void SDL_modText(t_window * window, int idtext, char * content, SDL_Color couleur, int x, int y) {

	if (window == NULL) return;
	if (window->nbText < idtext) return;
	if (window->windowText == NULL) return;
	
	window->windowText[idtext].couleur = couleur;
	window->windowText[idtext].content = content;
	
	window->windowText[idtext].x = x;
	window->windowText[idtext].y = y;
	
	return;

}

void SDL_delText(t_window * window, int idtext) {

	if (window == NULL) return;
	if (window->nbText < idtext) return;
	if (window->windowText == NULL) return;
	
	int i = 0;
	
	for (i = idtext; i < (window->nbText); i++) {
	
		window->windowText[i] = window->windowText[i+1];
	
	}
	
	window->nbText = (window->nbText)-1;
	
	return;
}

void SDL_UpdateEvents(Input* in)
{
	
	while(SDL_PollEvent(&GlobalEvent))
	{
		switch (GlobalEvent.type)
		{
			case SDL_KEYDOWN:
			
				if (buffer_deliver == 1) {
				
					in->key[GlobalEvent.key.keysym.sym]=1;
					buffer = GlobalEvent.key.keysym.unicode;
					buffer_deliver = 0; //Need to know if char was captured..!
				
				}
			
				break;
			
			case SDL_KEYUP:
			
				in->key[GlobalEvent.key.keysym.sym]=0;
				break;
			
			case SDL_MOUSEMOTION:
				in->mousex=GlobalEvent.motion.x;
				in->mousey=GlobalEvent.motion.y;
				in->mousexrel=GlobalEvent.motion.xrel;
				in->mouseyrel=GlobalEvent.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				in->mousebuttons[GlobalEvent.button.button]=1;
				break;
			case SDL_MOUSEBUTTONUP:
				in->mousebuttons[GlobalEvent.button.button]=0;
				break;
			case SDL_QUIT:
				in->quit = 1;
				break;
			default:
				break;
		}
	}
}

int SDL_IsMouseOver(t_window * window, int hauteur, int largeur, int x, int y) {
	
	if ( (in.mousey-(window->y)) > y && (in.mousey-(window->y)) <= y+hauteur && (in.mousex-(window->x)) > x && (in.mousex-(window->x)) <= x+largeur ) {
		return 1;
	}else{
		return 0;
	}
	
}

void SDL_loadRessources() {

	BTN_OVER = IMG_Load("ressources/images/m_bg_s1.png");
	BTN_NOTOVER = IMG_Load("ressources/images/m_bg_s0.png");
	FORM = IMG_Load("ressources/images/ch_saisie_actif.png");
	
	SELECT = Mix_LoadWAV("ressources/snd/select.wav");
	ENTER = Mix_LoadWAV("ressources/snd/enter.wav");
	
}

void SDL_loadWindow(t_window * window) {

	int i = 0;
	
	char saisie_content[100], texturePath[100]; //Form ONLY
	window->windowSurface = NULL;
	
	for (i = (window->nbImg_loaded); i < (window->nbImg); i++) {
		
		sprintf(texturePath, "ressources/images/%s", window->windowImg[i].file);
		window->windowImg[i].buffer = IMG_Load(texturePath);
		
	}
	
	window->nbImg_loaded = i;
	
	for (i = (window->nbObj_loaded); i < (window->nbObj); i++) {
	
		switch (window->windowObj[i].type) {
		
			case 0: //Simple btn
				
				window->windowObj[i].buffer_title = TTF_RenderText_Blended(ttf_police, window->windowObj[i].title, colorWhite);
				
				break;
				
			case 1: //Form
			
				memset (saisie_content, 0, sizeof (saisie_content));
				
				if (window->windowObj[i].MouseOver == 1) {
					
					strcpy (saisie_content, window->windowObj[i].dest);
  					strcat (saisie_content,"|");
  					
				}else{
				
					strcpy (saisie_content, window->windowObj[i].dest);
					
				}
	
				window->windowObj[i].buffer_content = TTF_RenderText_Blended(ttf_police, saisie_content, colorBlack);
				window->windowObj[i].buffer_title = TTF_RenderText_Blended(ttf_police, window->windowObj[i].title, colorWhite);
				
				break;
				
		}
	
	}
	
	window->nbObj_loaded = i;
	
	for (i = (window->nbText_loaded); i < (window->nbText); i++) {
		
		window->windowText[i].buffer = TTF_RenderText_Blended(ttf_police, window->windowText[i].content, window->windowText[i].couleur);
			
	}
	
	window->nbText_loaded = 0;

}

void SDL_BlitObjs(t_window * window) {

	int i = 0;
	
	SDL_Rect positionFond; 
	char saisie_content[100]; //Form ONLY
	
	if (window == NULL) return;
	
	window->windowSurface = SDL_CreateRGBSurface(0, window->height, window->width, 32, 0, 0, 0, 0);
	SDL_FillRect(window->windowSurface, NULL, SDL_MapRGB(window->windowSurface->format, 0, 0, 0));
	
	//Scan textures to Blit !
	for (i = 0; i < (window->nbImg); i++) {
		
		positionFond.x = window->windowImg[i].x;
		positionFond.y = window->windowImg[i].y;
				
		SDL_BlitSurface(window->windowImg[i].buffer, NULL, window->windowSurface, &positionFond);
		
	}
	
	//Blit OBJ ONLY
	for (i = 0; i < (window->nbObj); i++) {
	
		switch (window->windowObj[i].type) {
		
			case 0: //Simple btn
				
				positionFond.x = window->windowObj[i].x;
				positionFond.y = window->windowObj[i].y;
				
				//On charge l'image concern�e ++ si souris survol choix
				if (window->windowObj[i].MouseOver == 1) {
					
					//imageDeFond = IMG_Load("ressources/images/m_bg_s1.png");
					SDL_BlitSurface(BTN_OVER, NULL, window->windowSurface, &positionFond);
					
				}else{
				
					//imageDeFond = IMG_Load("ressources/images/m_bg_s0.png");
					SDL_BlitSurface(BTN_NOTOVER, NULL, window->windowSurface, &positionFond);
					
				}
				
				positionFond.x += 20;
				positionFond.y += 5;
				SDL_BlitSurface(window->windowObj[i].buffer_title, NULL, window->windowSurface, &positionFond);
				
				break;
				
			case 1: //Form
			
				memset (saisie_content, 0, sizeof (saisie_content));
				
				if (window->windowObj[i].MouseOver == 1) {
					
					strcpy (saisie_content, window->windowObj[i].dest);
  					strcat (saisie_content,"|");
  					
				}else{
				
					strcpy (saisie_content, window->windowObj[i].dest);
					
				}
				
				//imageDeFond = IMG_Load("ressources/images/ch_saisie_actif.png");
				
				positionFond.x = window->windowObj[i].x;
				positionFond.y = window->windowObj[i].y;
				
				SDL_BlitSurface(FORM, NULL, window->windowSurface, &positionFond);
				
				SDL_FreeSurface(window->windowObj[i].buffer_content);
				window->windowObj[i].buffer_content = TTF_RenderText_Blended(ttf_police, saisie_content, colorBlack);
				positionFond.x = (window->windowObj[i].x)+10;
				positionFond.y = (window->windowObj[i].y)+5;
				SDL_BlitSurface(window->windowObj[i].buffer_content, NULL, window->windowSurface, &positionFond);
	
				//titre_ttf = TTF_RenderText_Blended(ttf_police, window->windowObj[i].title, colorWhite);
				positionFond.x = (window->windowObj[i].x)-55;
				positionFond.y = (window->windowObj[i].y)+5;
				SDL_BlitSurface(window->windowObj[i].buffer_title, NULL, window->windowSurface, &positionFond);
				
				break;
				
		}
		
	}
	
	if (tff_loaded == 1) {
		//Scan text to Blit !
		for (i = 0; i < (window->nbText); i++) {
		
			//textsurf = TTF_RenderText_Blended(ttf_police, window->windowText[i].content, window->windowText[i].couleur);
			
			positionFond.x = window->windowText[i].x;
			positionFond.y = window->windowText[i].y;
			
			SDL_BlitSurface(window->windowText[i].buffer, NULL, window->windowSurface, &positionFond);
			
		}
	}
	
	positionFond.x = window->x;
	positionFond.y = window->y;
	SDL_BlitSurface(window->windowSurface, NULL, screen, &positionFond);
	
	SDL_FreeSurface(window->windowSurface);

}

int SDL_generateMenu(int nb_entree, char sommaire[N][M]) {
	
	int i = 0, MouseOverObj = 0, MouseOverObjPrev = 0, firstFrame = 0;
	
	t_window * menu = SDL_newWindow("Menu", 0, 0, 800, 600);
	
	for (i = 0; i < nb_entree; i++) {
		SDL_newObj(menu, NULL, 0, sommaire[i], NULL, ALL, 100, 100+(50*i), 40, 230);
	}
	
	SDL_newTexture(menu, NULL, "app_bg.png", 0, 0, 600, 800);
	SDL_newTexture(menu, NULL, "BarreLaterale.png", 80, 25, 0, 0);
	
	SDL_loadWindow(menu);
	
	while (1) {
		
		do {
			
			MouseOverObjPrev = MouseOverObj;
			SDL_UpdateEvents(&in);
			MouseOverObj = SDL_IsMouseOverObj(menu);
			
			if (firstFrame == 0) { firstFrame = 1; break; }
			SDL_Delay(50);
			
		} while ((MouseOverObjPrev == MouseOverObj) && (!in.mousebuttons[SDL_BUTTON_LEFT]) && (in.quit != 1));
		
		if (MouseOverObjPrev != MouseOverObj) {		
			SDL_BlitObjs(menu);
			SDL_Flip (screen);
			SDL_FreeSurface(screen);
		}
		
		if ((MouseOverObj != -1) && ((menu->windowObj[MouseOverObj].type) == 0)) {
			Mix_PlayChannel(-1, SELECT, 0);
		}
		
		//If user clic (left btn)
		if ((in.mousebuttons[SDL_BUTTON_LEFT]) && (MouseOverObj != -1)) {
		
			Mix_PlayChannel(-1, ENTER, 0);
			in.mousebuttons[SDL_BUTTON_LEFT] = 0;

			return MouseOverObj;
		
		}
		
		if (in.quit == 1) {
			exit(0);
		}
		
	}
	
}

int SDL_generate(t_window * window) {
	
	int MouseOverObj = 0, MouseOverObjPrev = 0, firstFrame = 0, forceFrame = 0;
	int uniqueFrame = 0;
	
	if (window == NULL) return -1;
	
	if ((window->nbObj) == 0) {
		uniqueFrame = 1;
	}
	
	SDL_loadWindow(window);
	
	while (1) {
		
		do {
			
			MouseOverObjPrev = MouseOverObj;
			SDL_UpdateEvents(&in);
			MouseOverObj = SDL_IsMouseOverObj(window);
			
			if (firstFrame == 0) { firstFrame = 1; break; }
			SDL_Delay(50);
			
		} while ((MouseOverObjPrev == MouseOverObj) && (!in.mousebuttons[SDL_BUTTON_LEFT]) && (buffer_deliver == 1) && (in.quit != 1) && (uniqueFrame != 1));
		
		if (buffer_deliver == 0) {
			SDL_CaptureForm(window, MouseOverObj);
			forceFrame = 1;
		}
		
		
		if ((MouseOverObjPrev != MouseOverObj) || (forceFrame == 1) || (uniqueFrame == 1)) {		
			SDL_BlitObjs(window);
			SDL_Flip (screen);
			SDL_FreeSurface(screen);
			forceFrame = 0;
			
			if (uniqueFrame == 1) {
				return 0;
			}
		}
		
		if ((MouseOverObj != -1) && ((window->windowObj[MouseOverObj].type) == 0)) {
			
			Mix_PlayChannel(-1, SELECT, 0);
			
		}
		
		//If user clic (left btn)
		if ((in.mousebuttons[SDL_BUTTON_LEFT]) && (MouseOverObj != -1) && ((window->windowObj[MouseOverObj].type) == 0) ) {
			
			Mix_PlayChannel(-1, ENTER, 0);
			in.mousebuttons[SDL_BUTTON_LEFT] = 0;

			return MouseOverObj;
		
		}
		
		if (in.quit == 1) {
			exit(0);
		}
		
	}
	
}

