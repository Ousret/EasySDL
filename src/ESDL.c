/**
 * \file ESDL.c
 * \brief EasySDL
 * \author TAHRI Ahmed, SIMON Jérémy
 * \version 0.5.2
 * \date 25-01-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ESDL.h"

/* SDL */
SDL_Surface *screen = NULL, *BTN_NOTOVER = NULL, *BTN_OVER = NULL, *FORM = NULL;
SDL_Color colorRed = {255, 0, 0, 0}, colorWhite = {255, 255, 255, 0}, colorBlack = {0, 0, 0, 0}, colorGreenLight = {90, 169, 120, 0};
SDL_Event GlobalEvent;
TTF_Font *ttf_police = NULL;
/* FMODex */
FMOD_SOUND *SELECT = NULL, *ENTER = NULL;
FMOD_SYSTEM *fmod_system = NULL;
t_audio * fmodbuffer;
/* Ref */
int buffer_deliver = 1, nbSnd = 0, ttf_loaded = 0, audio_loaded = 0, DELAY_EACH_FRAME = 50;
char * resSND = NULL, * resIMG = NULL, * resTTF = NULL, buffer = 0;
Input in;

void SDL_setDelaySingleFrame(int delay) { DELAY_EACH_FRAME = delay; }

int SDL_playSound(char * sndfile) {
	
	if (!audio_loaded) return 0;
	char filePath[256];
	int i = 0, alreadyLoaded = -1;
	
	sprintf(filePath, "%s%s", resSND, sndfile);
	
	for (i = 0; i < nbSnd; i++) {
		
		if (strcmp(fmodbuffer[i].file, sndfile) == 0)  {
			alreadyLoaded = i;
			break;
		}
			
	}
	
	if (alreadyLoaded != -1) {
		
		FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, fmodbuffer[alreadyLoaded].buffer, 0, NULL);
		return 1;
		
	}else{
		
		fprintf(stderr, "<! Warning> EasySDL: You need to load it before playing %s, ignoring..\n", filePath);
		return 0;
		
	}
	
}

int SDL_loadSound(char * sndfile) {
	
	if (!audio_loaded) return 0;
	char filePath[256];
	FMOD_SOUND * tmp_audio = NULL;
	t_audio * tmp_realloc = NULL;
	
	sprintf(filePath, "%s%s", resSND, sndfile);
	
	if (FMOD_System_CreateSound(fmod_system, filePath, FMOD_CREATESAMPLE, 0, &tmp_audio) != FMOD_OK) {
		fprintf(stderr, "<! Warning> EasySDL: FMODex cannot load sound file %s, ignoring..\n", filePath);
		return 0;
	}
	
	if (!fmodbuffer) {
		fmodbuffer = (t_audio*) malloc(sizeof(t_audio));
		
		if (!fmodbuffer) {
			FMOD_Sound_Release(tmp_audio);
			return 0;
		}
	}else{
		tmp_realloc = (t_audio*) realloc(fmodbuffer, sizeof(t_audio)*(nbSnd+1));
		
		if (tmp_realloc) {
			fmodbuffer = tmp_realloc;
		}else{
			FMOD_Sound_Release(tmp_audio);
			return 0; //Out of memory..!
		}
	}
	
	
	fmodbuffer[nbSnd].file = malloc(sizeof(char)*(strlen(sndfile)+1)); // strlen + 1 because of '\0' (end of string) !
	if (!(fmodbuffer[nbSnd].file)) return 0;
	
	strcpy(fmodbuffer[nbSnd].file, sndfile);
	fmodbuffer[nbSnd].buffer = tmp_audio;
	
	nbSnd++;
	
	return 1;
	
}

int SDL_unloadSound(char * sndfile) {
	
	if (!audio_loaded) return 0;
	int i = 0, alreadyLoaded = -1;
	
	char * tmp_file = NULL;
	FMOD_SOUND * tmp_audio = NULL;
	t_audio * tmp_realloc = NULL;
	
	for (i = 0; i < nbSnd; i++) {
		if (strcmp(fmodbuffer[i].file, sndfile) == 0) 
			alreadyLoaded = i;
			break;
	}
	
	if (alreadyLoaded != -1) {
		
		tmp_audio = fmodbuffer[alreadyLoaded].buffer;
		tmp_file = fmodbuffer[alreadyLoaded].file;
		
		for (i = alreadyLoaded; i < (nbSnd-1); i++) {
			fmodbuffer[i] = fmodbuffer[i+1];
		}
		
		if (tmp_audio) {
			FMOD_Sound_Release(tmp_audio);
		}
		
		if (tmp_file) {
			free (tmp_file);
		}
		
		tmp_realloc = realloc(fmodbuffer, sizeof(t_audio)*nbSnd);
		if (tmp_realloc) fmodbuffer = tmp_realloc;
		nbSnd--;
		
		return 1;
	}
	
	fprintf(stderr, "<! Warning> EasySDL: Unable to unload %s, ignoring..\n", sndfile);
	return 0;
	
}

int SDL_unloadallSound() {
	
	if (!audio_loaded) return 0;
	if (!fmodbuffer || !nbSnd) return 0;
	int i = 0;
	
	for (i = 0; i < nbSnd; i++) {
		
		if (fmodbuffer[i].file) free (fmodbuffer[i].file);
		if (fmodbuffer[i].buffer) FMOD_Sound_Release(fmodbuffer[i].buffer);
		
	}
	
	if (fmodbuffer) {
		free (fmodbuffer);
		fmodbuffer = NULL;
	}
	nbSnd = 0;
	
	return 1;
	
}

int SDL_nbObj(t_context * context) {
	if (!context) return 0;
	return (context->nbObj);
}

int SDL_nbText(t_context * context) {
	if (!context) return 0;
	return (context->nbText);
}

int SDL_nbImage(t_context * context) {
	if (!context) return 0;
	return (context->nbImg);
}

int SDL_nbSprite(t_context * context) {
	if (!context) return 0;
	return (context->nbSprite);
}

int SDL_contextEmpty(t_context * context) {
	if (!context) return 1;
	if ((SDL_nbObj(context) + SDL_nbText(context) + SDL_nbImage(context)) == 0) return 1;
	return 0;
}

int SDL_requestExit() {
	return (in.quit);
}

int SDL_ismouseover(t_context * context, t_typeData type) {
	
	int i = 0, overobj = -1;
	if (!context) return overobj;
	
	SDL_UpdateEvents(&in);
	
	switch (type) {
	
		case BUTTON:
			
			if (!(context->contextObj) || !(context->nbObj)) return overobj;
			
			for (i = 0; i < (context->nbObj); i++) {
				
				if (context->contextObj[i].type == BUTTON) {
					if (SDL_ismouseoverArea(context, BTN_NOTOVER->h, BTN_NOTOVER->w, context->contextObj[i].x, context->contextObj[i].y)) {
						context->contextObj[i].MouseOver = 1;
						overobj = i;
					}else{
						context->contextObj[i].MouseOver = 0;
					}
				}
				
			}
			
			break;
		case INPUT:
			
			if (!(context->contextObj) || !(context->nbObj)) return overobj;
			
			for (i = 0; i < (context->nbObj); i++) {
				
				if (context->contextObj[i].type == INPUT) {
					if (SDL_ismouseoverArea(context, FORM->h, FORM->w, context->contextObj[i].x, context->contextObj[i].y)) {
						context->contextObj[i].MouseOver = 1;
						overobj = i;
					}else{
						context->contextObj[i].MouseOver = 0;
					}
				}
				
			}
			
			break;
		case IMG:
		
			if (!(context->contextImg) || !(context->nbImg)) return overobj;
			
			for (i = 0; i < (context->nbImg); i++) {
				
				if (SDL_ismouseoverArea(context, context->contextImg[i].buffer->h, context->contextImg[i].buffer->w, context->contextImg[i].x, context->contextImg[i].y)) {
					overobj = i;
				}
				
			}
			
			break;
		case TEXT:
			
			if (!(context->contextText) || !(context->nbText)) return overobj;
			
			for (i = 0; i < (context->nbText); i++) {
				
				if (SDL_ismouseoverArea(context, context->contextText[i].buffer->h, context->contextText[i].buffer->w, context->contextText[i].x, context->contextText[i].y)) {
					overobj = i;
				}
				
			}
			
			break;
		case SPRITE:
			
			if (!(context->contextSprite) || !(context->nbSprite)) return overobj;
			
			for (i = 0; i < (context->nbSprite); i++) {
				
				if (SDL_ismouseoverArea(context, context->contextSprite[i].sp_height, context->contextSprite[i].sp_width, context->contextSprite[i].x, context->contextSprite[i].y)) {
					overobj = i;
				}
				
			}
			
			break;
			
		case RECTANGLE:
			
			if (!(context->contextRect) || !(context->nbRect)) return overobj;
			
			for (i = 0; i < (context->nbRect); i++) {
				
				if (SDL_ismouseoverArea(context, context->contextRect[i].def.h, context->contextRect[i].def.w, context->contextRect[i].def.x, context->contextRect[i].def.y)) {
					overobj = i;
				}
				
			}
			
			break;
	
	}
	
	return overobj; 
	
}

void SDL_setSNDFolder(char * newFolder) {
	if (newFolder) resSND = newFolder;
}

void SDL_setTTFFolder(char * newFolder) {
	if (newFolder) resTTF = newFolder;
}

void SDL_setIMGFolder(char * newFolder) {
	if (newFolder) resIMG = newFolder;
}

void SDL_initWindow(int width, int height, int fullscreen, char * title, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support) {
	
    int sdl_start = 0;
	char file[256] = ""; //Generating file path
	
	if (!resSND) SDL_setSNDFolder("ressources/snd/");
	if (!resTTF) SDL_setTTFFolder("ressources/ttf/");
	if (!resIMG) SDL_setIMGFolder("ressources/images/");
	
    /* Initialize SDL */
    sdl_start = SDL_Init (SDL_INIT_VIDEO);
    
    if (sdl_start < 0)
    {
        
        fprintf (stderr, "<! Fatal> EasySDL: SDL failed to load because of: %s\n", SDL_GetError ());
        exit (1);
        
    }
    
	atexit (SDL_unload);
	
	if (fullscreen) {
		screen = SDL_SetVideoMode (width, height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	}else{
		screen = SDL_SetVideoMode (width, height, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
	}
    
    if (!screen)
    {
        fprintf (stderr, "<! Fatal> EasySDL: Unable to load context at %ix%i in 16 bits': %s\n", width, height ,SDL_GetError ());
        exit (2);
    }
    
	SDL_WM_SetCaption (title, NULL);
	
	if (icon_name) {
		SDL_WM_SetIcon(IMG_Load(icon_name), NULL);
	}
	
	if (audio_support == 1) {
		
		FMOD_System_Create(&fmod_system);
    	FMOD_System_Init(fmod_system, 2, FMOD_INIT_NORMAL, NULL);
    	
		audio_loaded = 1;
	}
	
	if (ttf_support == 1) {
		
		if(TTF_Init() == -1)
		{
	    	fprintf (stderr, "<! Fatal> EasySDL: Cannot load SDL_ttf, you may want to check your SDL setup !\n");
	        exit (1);
		}
		
		sprintf(file, "%s%s", resTTF, police_name);
		
		if (police_size <= 0) {
			fprintf(stderr, "<! Fatal> EasySDL: Cannot load ttf_police with size=0 !\n");
			exit (1);
		}
		
		ttf_police = TTF_OpenFont(file, police_size);
		if (!ttf_police) {
			fprintf(stderr, "<! Fatal> EasySDL: Cannot load %s\n<?> EasySDL: Exit 0\n", file);
			exit(0); 
		}
		ttf_loaded = 1;
	}
	
	SDL_EnableUNICODE(1);
	
	int flags=IMG_INIT_JPG|IMG_INIT_PNG;
	int initted=IMG_Init(flags);
	
	if((initted&flags) != flags) {
    	fprintf(stdout, "<! Fatal> EasySDL: IMG_Init: Failed to init required support!\n");
    	fprintf(stdout, "<! Fatal> EasySDL: IMG_Init: %s\n", IMG_GetError());
    	exit (1);
	}
	
	SDL_loadRessources();
	
}

int SDL_captureInput(t_context * context, int obj) {

	int current_len = 0;
	//Return if there's nothing to process
	if (context == NULL) return 0;
	
	if ((obj != -1) && (context->contextObj[obj].dest != NULL) && (buffer_deliver == 0)) {
			
		if ((context->contextObj[obj].type) == 1) {
			
			current_len = strlen(context->contextObj[obj].dest);
			
			if (current_len < 35) {
				
				if (buffer == '\b' || buffer == 127) { //127 pour clavier MacOS X BT
					
					if (current_len > 0) {
							
						context->contextObj[obj].dest[current_len-1] = '\0';
							
					}
					
				}else {
					
					switch (context->contextObj[obj].typeForm) {
					
						case NUMERIC:
							if (buffer >= '0' && buffer <= '9') {
								context->contextObj[obj].dest[current_len] = buffer;
								context->contextObj[obj].dest[current_len+1] = '\0';
							}
							break;
							
						case ALPHA:
							if ((buffer >= 'a' && buffer <= 'z') || (buffer >= 'A' && buffer <= 'Z')) {
								context->contextObj[obj].dest[current_len] = buffer;
								context->contextObj[obj].dest[current_len+1] = '\0';
							}
							
							break;
							
						case ALPHANUMERIC:
							if ((buffer >= 'a' && buffer <= 'z') || (buffer >= 'A' && buffer <= 'Z') || (buffer >= '0' && buffer <= '9')) {
								context->contextObj[obj].dest[current_len] = buffer;
								context->contextObj[obj].dest[current_len+1] = '\0';
							}
							
							break;
							
						case NOMASK:
							
							context->contextObj[obj].dest[current_len] = buffer;
							context->contextObj[obj].dest[current_len+1] = '\0';
							break;
							
						default:
						
							context->contextObj[obj].dest[current_len] = buffer;
							context->contextObj[obj].dest[current_len+1] = '\0';
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

t_context * SDL_newContext(char * title, int x, int y, int height, int width) {
	
	t_context * tmp = (t_context*) malloc(sizeof(t_context));
	tmp->title = title;
	
	tmp->contextSurface = NULL;
	
	tmp->contextObj = NULL;
	tmp->contextText = NULL;
	tmp->contextImg = NULL;
	tmp->contextSprite = NULL;
	tmp->contextRect = NULL;
	
	tmp->nbObj = 0;
	tmp->nbText = 0;
	tmp->nbImg = 0;
	tmp->nbSprite = 0;
	tmp->nbRect = 0;
	
	tmp->x = x;
	tmp->y = y;
	
	tmp->height = height;
	tmp->width = width;
	
	return tmp;

}

void SDL_freeContext(t_context * context) {
	
	if (!context) return;
	int i = 0, j = 0, multipleLoad = 0;
	
	if (context->contextObj) {
		for (i = 0;i < (context->nbObj); i ++) {
			if (context->contextObj[i].buffer_title) {
				
				SDL_FreeSurface(context->contextObj[i].buffer_title);
				context->contextObj[i].buffer_title = NULL;
				
			}
				
			if (context->contextObj[i].buffer_content) {
				SDL_FreeSurface(context->contextObj[i].buffer_content);
				context->contextObj[i].buffer_content = NULL;
			}
				
		}
		free (context->contextObj);
	}
	
	if (context->contextText) {
		for (i = 0;i < (context->nbText); i ++) {
			if (context->contextText[i].buffer) {
				SDL_FreeSurface(context->contextText[i].buffer);
				context->contextText[i].buffer = NULL;
			}
				
		}
		free (context->contextText);
	}
	
	if (context->contextImg) {
		//tmp = context->nbImg;
		for (i = 0;i < (context->nbImg); i++) {
			if (context->contextImg[i].buffer) {
				for (j = i; j < (context->nbImg); j++) {
					if (strcmp(context->contextImg[i].file, context->contextImg[j].file) == 0) {
						if (j != i) {
							multipleLoad = 1;
							break;
						}
					}
				}
				
				if (!multipleLoad) { 
					SDL_FreeSurface(context->contextImg[i].buffer);
				}else{
					multipleLoad = 0;
				}
				
				context->contextImg[i].buffer = NULL;
			}
			
			if (context->contextImg[i].file) {
				free (context->contextImg[i].file);
				context->contextImg[i].file = NULL;
			}
				
		}
		free (context->contextImg);
	}
	
	if (context->contextSprite) {
		for (i = 0; i < (context->nbSprite); i++) {
			
			if (context->contextSprite[i].buffer) {
				SDL_FreeSurface(context->contextSprite[i].buffer);
				context->contextSprite[i].buffer = NULL;
			}
				
		}
		free (context->contextSprite);
	}
	
	if (context->contextRect) {
		free (context->contextRect);
	}
	
	free (context);
	
}

int SDL_newRect(t_context *context, int * idrect, SDL_Color color, int height, int width, int x, int y) {
	
	t_rect *n_realloc = NULL;
	
	if (!context) return 0;
	if (!height || !width) return 0;
	
	if (!(context->contextRect)) {
		
		context->contextRect = (t_rect*) malloc(sizeof(t_rect));
		if (!(context->contextRect)) return 0;
		
	}else{
		n_realloc = realloc(context->contextRect, sizeof(t_rect)*((context->nbRect) +1));
		if (n_realloc) {
			context->contextRect = n_realloc;
		}else{
			return 0; /* Out of memory */
		}
	}
	
	context->contextRect[context->nbRect].color = color;
	context->contextRect[context->nbRect].def.h = height;
	context->contextRect[context->nbRect].def.w = width;
	context->contextRect[context->nbRect].def.x = x;
	context->contextRect[context->nbRect].def.y = y;
	
	if (idrect) {
		*idrect = context->nbRect;
	}
	
	context->nbRect = (context->nbRect) + 1;
	
	return 1;
	
}

int SDL_editRect(t_context *context, int idrect, SDL_Color color, int height, int width, int x, int y) {
	
	if (!context || !(context->contextRect) || idrect <= (context->nbRect)) return 0;
	
	context->contextRect[idrect].color = color;
	context->contextRect[idrect].def.h = height;
	context->contextRect[idrect].def.w = width;
	context->contextRect[idrect].def.x = x;
	context->contextRect[idrect].def.y = y;
	
	
	return 1;
	
}

int SDL_delRect(t_context *context, int idrect) {
	
	if (!context || !(context->contextRect) || idrect <= (context->nbRect)) return 0;
	t_rect * n_realloc = NULL;
	int i = 0;
	
	if (context->contextRect[idrect].id) *(context->contextRect[idrect].id) = -1;
	
	for (i = idrect; i < (context->nbRect)-1; i++) {
		
		context->contextRect[i] = context->contextRect[i+1];
		if (context->contextRect[i].id) *(context->contextRect[i].id) = *(context->contextRect[i].id) - 1;
		
	}
	
	n_realloc = realloc(context->contextRect, sizeof(t_rect)*(context->nbRect));
	
	if (n_realloc) {
		context->contextRect = n_realloc;
	}
	
	context->nbRect = (context->nbRect) - 1;
	
	return 1;
	
}

int SDL_newSprite(t_context *context, char * filename, SDL_Color transparancy, int sp_height, int sp_width, int x, int y, int position, int animation, int hide) {
	
	t_sprite * n_realloc = NULL;
	SDL_Surface *tmp = NULL;
	char texturePath[150];
	
	if (!context) return 0;
	if (!strlen(filename)) return 0;
	
	/* Check if file exist or not.. */
	sprintf(texturePath, "%s%s", resIMG ,filename);
	tmp = IMG_Load(texturePath);
	
	if (!tmp) {
		fprintf(stdout, "<! Warning> EasySDL: Cannot load sprite %s, ignoring..\n", texturePath);
		return 0;
	}
	
	if (!(context->nbSprite)) {
		if (!(context->contextSprite)) {
			context->contextSprite = (t_sprite*) malloc(sizeof(t_sprite));
		}else{
			return 0;
		}
	}else{
		n_realloc = (t_sprite*) realloc(context->contextSprite, sizeof(t_sprite) * ((context->nbSprite)+1));
		
		if (n_realloc) {
			context->contextSprite = n_realloc;
		}else{
			return 0;
		}
	}
	
	context->contextSprite[context->nbSprite].buffer = tmp;
	
	/* Does not work for now.. need help for this one */
	SDL_SetColorKey( context->contextSprite[context->nbSprite].buffer, SDL_SRCCOLORKEY, SDL_MapRGB( context->contextSprite[context->nbSprite].buffer->format, transparancy.r, transparancy.g, transparancy.b ) );
	
	context->contextSprite[context->nbSprite].sp_height = sp_height;
	context->contextSprite[context->nbSprite].sp_width = sp_width;
	
	context->contextSprite[context->nbSprite].x = x;
	context->contextSprite[context->nbSprite].y = y;
	context->contextSprite[context->nbSprite].position = position;
	context->contextSprite[context->nbSprite].animation = animation;
	context->contextSprite[context->nbSprite].hide = hide;
	
	context->nbSprite = (context->nbSprite)+1;
	
	return 1;
}

int SDL_editSprite(t_context *context, int idSprite, int x, int y, int position, int animation, int hide) {

	if (!context) return 0;
	if (idSprite >= (context->nbSprite)) return 0;
	
	context->contextSprite[idSprite].x = x;
	context->contextSprite[idSprite].y = y;
	
	context->contextSprite[idSprite].position = position;
	context->contextSprite[idSprite].animation = animation;
	
	context->contextSprite[idSprite].hide = hide;
	
	return 1;
	
}

int SDL_delSprite(t_context *context, int idSprite) {

	int i = 0;

	if (!context) return 0;
	if (idSprite >= (context->nbSprite)) return 0;
	
	if (context->contextSprite[idSprite].buffer) {
		SDL_FreeSurface(context->contextSprite[idSprite].buffer);
		context->contextSprite[idSprite].buffer = NULL;
	}
	
	for (i = idSprite; i < (context->nbSprite)-1; i++) {
	
		context->contextSprite[i] = context->contextSprite[i+1];
	
	}
	
	context->nbSprite = (context->nbSprite)-1;
	
	context->contextSprite = (t_sprite*) realloc(context->contextSprite, sizeof(t_sprite)*(context->nbSprite));
	
	return 1;
}

int SDL_newObj(t_context * context, int * id, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y) {
	
	t_object * n_realloc = NULL;
	if (!ttf_loaded) return 0;
	
	if (!context) {
		fprintf(stderr, "<! Warning> EasySDL: Want to add object to undefined context, ignoring..\n");
		return 0;
	}
	if (strlen(title) == 0) return 0;
	
	if (context->nbObj == 0) {
		
		if (context->contextObj == NULL) {
			context->contextObj = (t_object*) malloc(sizeof(t_object));
		}else{
			return 0;
		}
		
	}else{
		
		n_realloc = (t_object*) realloc(context->contextObj, sizeof(t_object) * ((context->nbObj)+1));
		if (n_realloc) {
			context->contextObj = n_realloc;
		}else{
			fprintf(stderr, "<! Error> EasySDL: Out of memory when trying to add object \"%s\" to %s, bad sign..\n", title, context->title);
			return 0; /* Out of memory */
		}
		
	}
	
	context->contextObj[context->nbObj].x = x;
	context->contextObj[context->nbObj].y = y;
	
	context->contextObj[context->nbObj].align = align;
	
	context->contextObj[context->nbObj].MouseOver = 0;
	context->contextObj[context->nbObj].buffer_title = TTF_RenderText_Blended(ttf_police, title, colorWhite);
	context->contextObj[context->nbObj].buffer_content = NULL;
	
	strcpy(context->contextObj[context->nbObj].title, title);
	
	context->contextObj[context->nbObj].type = type;
	
	if (type == 1) {
		
		context->contextObj[context->nbObj].dest = dest;
		context->contextObj[context->nbObj].typeForm = typeForm;
		
	}else {
		
		context->contextObj[context->nbObj].dest = NULL;
		context->contextObj[context->nbObj].typeForm = NONE;
		
	}
	
	context->contextObj[context->nbObj].id = NULL;
	
	if (id != NULL) {
		*id = context->nbObj;
		context->contextObj[context->nbObj].id = id;
	}
	context->nbObj = (context->nbObj+1);
	
	return 1;
	
}

int SDL_newImage(t_context * context, int * id, char * file, int x, int y) {

	char texturePath[150];
	t_image * n_realloc = NULL;
	SDL_Surface *tmp = NULL;
	int alreadyLoaded = -1, i = 0;
	if (!context) return 0;
	
	/* Check if not already loaded */
	for (i = 0; i < context->nbImg; i++) {
		if (strcmp(context->contextImg[i].file, file) == 0) {
			alreadyLoaded = i;
			break;
		}
	}
	
	sprintf(texturePath, "%s%s", resIMG, file);
	
	if (alreadyLoaded != -1) {
		tmp = context->contextImg[alreadyLoaded].buffer;
	}else{
		tmp = IMG_Load(texturePath);
	}
	
	if (!tmp) {
		fprintf(stderr, "<! Warning> EasySDL: Cannot load image %s, ignoring..\n", texturePath);
		return 0;
	}
	
	if (!(context->nbImg)) {
	
		if (!(context->contextImg)) {
		
			context->contextImg = (t_image*) malloc(sizeof(t_image));
			
		}else{
			
			fprintf(stderr, "<! Error> Count 0 image but context image area in %s seem not empty, reseting area..\n", context->title);
			context->contextImg = NULL;
			return 0;
			
		}
	
	}else{
	
		n_realloc = (t_image*) realloc(context->contextImg, sizeof(t_image) * ((context->nbImg)+1));
		if (n_realloc) {
			context->contextImg = n_realloc;
		}else{
			fprintf(stderr, "<! Error> Out of memory when trying to add image %s to %s, bad sign..\n", texturePath, context->title);
			return 0;
		}
		
	}
	
	context->contextImg[context->nbImg].x = x;
	context->contextImg[context->nbImg].y = y;
	context->contextImg[context->nbImg].file = malloc(sizeof(char)*(strlen(file)+1));
	strcpy(context->contextImg[context->nbImg].file, file);
	context->contextImg[context->nbImg].buffer = tmp;
	context->contextImg[context->nbImg].id = NULL;
	
	if (id != NULL) {
		*id = (context->nbImg);
		context->contextImg[context->nbImg].id = id;
	}
	
	context->nbImg = (context->nbImg)+1;
	
	return 1;

}

int SDL_editImage(t_context * context, int idimg, int x, int y) {
	
	if (!context) {
		fprintf(stderr, "<! Error> EasySDL: Invalid context when trying to modify image id = %i, ignoring..\n", idimg);
		return 0;
	}
	if (!(context->contextImg)) {
		fprintf(stderr, "<! Error> EasySDL: No image were allocated in %s when trying to modify image id = %i, ignoring..\n", context->title, idimg);
		return 0;
	}
	if (context->nbImg <= idimg) {
		fprintf(stderr, "<! Error> EasySDL: Want to modify image id = %i in %s with only %i image(s) loaded, ignoring..\n", idimg, context->title, context->nbImg);
		return 0;
	}
	
	context->contextImg[idimg].x = x;
	context->contextImg[idimg].y = y;
	
	return 1;

}

int SDL_delImage(t_context * context, int idimg) {

	if (!context) {
		fprintf(stderr, "<! Error> EasySDL: Invalid context when trying to delete image id = %i, ignoring..\n", idimg);
		return 0;
	}
	if (!(context->contextImg)) {
		fprintf(stderr, "<! Error> EasySDL: No image were allocated in %s when trying to delete image id = %i, ignoring..\n", context->title, idimg);
		return 0;
	}
	if (context->nbImg <= idimg) {
		fprintf(stderr, "<! Error> EasySDL: Want to delete image id = %i in %s with only %i image(s) loaded, ignoring..\n", idimg, context->title, context->nbImg);
		return 0;
	}
	
	int i = 0, multipleLoad = 0;
	
	for (i = 0; i < context->nbImg; i++) {
		if (strcmp(context->contextImg[i].file, context->contextImg[idimg].file) == 0) {
			if (i != idimg) {
				multipleLoad = 1;
				break;
			}
		}
	}
	
	
	if (context->contextImg[idimg].buffer && !multipleLoad) {
		SDL_FreeSurface(context->contextImg[idimg].buffer);
		context->contextImg[idimg].buffer = NULL;
	}
	
	if (context->contextImg[idimg].file) {
		free (context->contextImg[idimg].file);
		context->contextImg[idimg].file = NULL;
	}
	
	if (context->contextImg[idimg].id) *(context->contextImg[idimg].id) = -1;
	
	for (i = idimg; i < (context->nbImg)-1; i++) {
		context->contextImg[i] = context->contextImg[i+1];
		if (context->contextImg[i].id) *(context->contextImg[i].id) = *(context->contextImg[i].id) - 1;
	}
	
	context->contextImg = (t_image*) realloc(context->contextImg, sizeof(t_image)*(context->nbImg));
	context->nbImg = (context->nbImg)-1;
	
	return 1;

}

int SDL_editObj(t_context * context, int obj, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y) {

	if (!context) {
		fprintf(stderr, "<! Error> EasySDL: Invalid context when trying to modify object id = %i, ignoring..\n", obj);
		return 0;
	}
	if (context->nbObj <= obj) {
		fprintf(stderr, "<! Error> EasySDL: Want to change obj id %i in %s but there are only %i obj loaded, ignoring..\n", obj, context->title ,context->nbObj);
		return 0;
	}
	if (context->contextObj == NULL) {
		fprintf(stderr, "<! Error> EasySDL: No object were allocated in %s when trying to modify obj id = %i, ignoring..\n", context->title ,obj);
		return 0;
	}
	context->contextObj[obj].x = x;
	context->contextObj[obj].y = y;
	context->contextObj[obj].MouseOver = 0;
	
	strcpy(context->contextObj[obj].title, title);
	context->contextObj[obj].align = align;
	
	context->contextObj[obj].type = type;
	
	if (type == 1) {
		
		context->contextObj[obj].dest = dest;
		context->contextObj[obj].typeForm = typeForm;
		
	}else {
		
		context->contextObj[obj].dest = NULL;
		context->contextObj[obj].typeForm = NONE;
		
	}
	
	if (context->contextObj[obj].buffer_title) {
		SDL_FreeSurface(context->contextObj[obj].buffer_title);
		context->contextObj[obj].buffer_title = NULL;
	}
	
	if (context->contextObj[obj].buffer_content) {
		SDL_FreeSurface(context->contextObj[obj].buffer_content);
		context->contextObj[obj].buffer_content = NULL;
	}
	
	context->contextObj[obj].buffer_title = TTF_RenderText_Blended(ttf_police, title, colorWhite);
	
	return 1;
	
}

int SDL_delObj(t_context * context, int obj) {

	if (context == NULL) return 0;
	if (context->nbObj <= obj) return 0;
	if (context->contextObj == NULL) return 0;
	
	int i = 0;
	
	if (context->contextObj[obj].buffer_title) {
		SDL_FreeSurface(context->contextObj[obj].buffer_title);
		context->contextObj[obj].buffer_title = NULL;
	}
	
	if (context->contextObj[obj].buffer_content) {
		
		SDL_FreeSurface(context->contextObj[obj].buffer_content);
		context->contextObj[obj].buffer_content = NULL;
		
	}
	
	if (context->contextObj[obj].id) *(context->contextObj[obj].id) = -1;
	
	for (i = obj; i < (context->nbObj)-1; i++) {
	
		context->contextObj[i] = context->contextObj[i+1];
		if (context->contextObj[i].id) *(context->contextObj[i].id) = *(context->contextObj[i].id) - 1;
		
	}
	
	context->contextObj = (t_object*) realloc(context->contextObj, sizeof(t_object)*(context->nbObj));
	context->nbObj = (context->nbObj)-1;
	
	return 1;

}

int SDL_newText(t_context * context, int * id, char * content, SDL_Color couleur, int x, int y) {
	
	t_text * n_realloc = NULL;
	
	if (!context || !ttf_loaded) return 0;
	
	if (context->nbText == 0) {
	
		if (context->contextText == NULL) {
			context->contextText = (t_text*) malloc(sizeof(t_text));
		}else{
			return 0;
		}
	
	}else{
		  n_realloc = (t_text*) realloc(context->contextText, sizeof(t_text) * ((context->nbText)+1));
		  
		  if (n_realloc) {
		  	context->contextText = n_realloc;
		  }else{
		  	return 0;
		  }
		  
	}
	
	context->contextText[context->nbText].couleur = couleur;
	context->contextText[context->nbText].content = content;
	
	context->contextText[context->nbText].x = x;
	context->contextText[context->nbText].y = y;
	
	replaceinstring(content, '\n', ' ');
	replaceinstring(content, '\t', ' ');
	
	context->contextText[context->nbText].buffer = TTF_RenderText_Blended(ttf_police, content, couleur);
	
	context->contextText[context->nbText].id = NULL;
	
	if (id != NULL) {
		*id = (context->nbText);
		context->contextText[context->nbText].id = id;
	}
	context->nbText = (context->nbText)+1;
	
	return 1;

}

int SDL_editText(t_context * context, int idtext, char * content, SDL_Color couleur, int x, int y) {

	if (context == NULL) return 0;
	if (context->nbText <= idtext) return 0;
	if (!(context->contextText)) return 0;
	
	context->contextText[idtext].couleur = couleur;
	if (content) context->contextText[idtext].content = content;
	
	if (x != -1) context->contextText[idtext].x = x;
	if (y != -1) context->contextText[idtext].y = y;
	
	//Free old buffer
	if(context->contextText[idtext].buffer) {
		SDL_FreeSurface(context->contextText[idtext].buffer);
		context->contextText[idtext].buffer = NULL;
	}
	
	context->contextText[idtext].buffer = TTF_RenderText_Blended(ttf_police, content, couleur);
	
	return 1;

}

int SDL_delText(t_context * context, int idtext) {

	if (context == NULL) return 0;
	if (context->nbText <= idtext) return 0;
	if (context->contextText == NULL) return 0;
	
	int i = 0;
	
	if (context->contextText[idtext].buffer) {
		SDL_FreeSurface(context->contextText[idtext].buffer);
		context->contextText[idtext].buffer = NULL;
	}
	
	if (context->contextText[idtext].id) *(context->contextText[idtext].id) = -1;
	
	for (i = idtext; i < (context->nbText)-1; i++) {
	
		context->contextText[i] = context->contextText[i+1];
		if (context->contextText[i].id) *(context->contextText[i].id) = *(context->contextText[i].id) - 1;
		
	}
	
	context->contextText = (t_text*) realloc(context->contextText, sizeof(t_text)*(context->nbText));
	context->nbText = (context->nbText)-1;
	
	return 1;
}

int SDL_isKeyPressed(int KEY_S) {
	SDL_UpdateEvents(&in);
	return in.key[KEY_S];
}

int SDL_isMousePressed(int MOUSE_S) {
	SDL_UpdateEvents(&in);
	return in.mousebuttons[MOUSE_S];
}

int SDL_getmousex() { 
	SDL_UpdateEvents(&in);
	return (in.mousex); 
}
int SDL_getmousey() { 
	SDL_UpdateEvents(&in);
	return (in.mousey); 
}

int SDL_getimagewidth(t_context * context, int imgid) {
	if (!context || (imgid >= context->nbImg) || !(context->contextImg[imgid].buffer)) return 0;
	return (context->contextImg[imgid].buffer->w);
}

int SDL_getimageheight(t_context * context, int imgid) {
	if (!context || (imgid >= context->nbImg) || !(context->contextImg[imgid].buffer)) return 0;
	return (context->contextImg[imgid].buffer->h);
}

int SDL_getposx(t_context * context, int id, t_typeData type) {
	
	if (!context) return 0;
	
	switch (type) {
	
		case BUTTON:
			if (!(context->contextObj) || (id >= context->nbObj) || (context->contextObj[id].type != type)) return 0;
			return (context->contextObj[id].x);
		case SPRITE:
			if (!(context->contextSprite) || (id >= context->nbSprite)) return 0;
			return (context->contextSprite[id].x);
		case IMG:
			if (!(context->contextImg) || (id >= context->nbImg)) return 0;
			return (context->contextImg[id].x);
		case TEXT:
			if (!(context->contextText) || (id >= context->nbText)) return 0;
			return (context->contextText[id].x);
		case INPUT:
			if (!(context->contextObj) || (id >= context->nbObj) || (context->contextObj[id].type != type)) return 0;
			return (context->contextObj[id].x);
		case RECTANGLE:
			if (!(context->contextRect) || (id >= context->nbRect)) return 0;
			return (context->contextRect[id].def.x);
		default:
			return 0;
			
	}
	
}

int SDL_getposy(t_context * context, int id, t_typeData type) {
	
	if (!context) return 0;
	
	switch (type) {
	
		case BUTTON:
			if (!(context->contextObj) || (id >= context->nbObj) || (context->contextObj[id].type != type)) return 0;
			return (context->contextObj[id].y);
		case SPRITE:
			if (!(context->contextSprite) || (id >= context->nbSprite)) return 0;
			return (context->contextSprite[id].y);
		case IMG:
			if (!(context->contextImg) || (id >= context->nbImg)) return 0;
			return (context->contextImg[id].y);
		case TEXT:
			if (!(context->contextText) || (id >= context->nbText)) return 0;
			return (context->contextText[id].y);
		case INPUT:
			if (!(context->contextObj) || (id >= context->nbObj) || (context->contextObj[id].type != type)) return 0;
			return (context->contextObj[id].y);
		case RECTANGLE:
			if (!(context->contextRect) || (id >= context->nbRect)) return 0;
			return (context->contextRect[id].def.y);
		default:
			return 0;
	}
	
}

void SDL_UpdateEvents(Input* in)
{
	
	while(SDL_PollEvent(&GlobalEvent))
	{
		switch (GlobalEvent.type)
		{
			case SDL_KEYDOWN:
			
				in->key[GlobalEvent.key.keysym.sym]=1;
			
				if (buffer_deliver == 1) {
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

int SDL_ismouseoverArea(t_context * context, int height, int width, int x, int y) {
	
	if ( (in.mousey-(context->y)) > y && (in.mousey-(context->y)) <= y+height && (in.mousex-(context->x)) > x && (in.mousex-(context->x)) <= x+width ) {
		return 1;
	}else{
		return 0;
	}
	
}

void SDL_loadRessources() {
	
	char filePath[256];
	
	sprintf(filePath, "%s%s", resIMG, "m_bg_s1.png");
	BTN_OVER = IMG_Load(filePath);
	sprintf(filePath, "%s%s", resIMG, "m_bg_s0.png");
	BTN_NOTOVER = IMG_Load(filePath);
	sprintf(filePath, "%s%s", resIMG, "ch_saisie_actif.png");
	FORM = IMG_Load(filePath);
	
	if (!BTN_OVER || !BTN_NOTOVER || !FORM) {
		fprintf(stderr, "<! Fatal> EasySDL: Missing image ressources in %s please check for m_bg_s1.png, m_bg_s0.png and ch_saisie_actif.png !\n", resIMG);
		exit(0);
	}
	
	if (audio_loaded) {
		
		sprintf(filePath, "%s%s", resSND, "select.wav");
		FMOD_System_CreateSound(fmod_system, filePath, FMOD_CREATESAMPLE, 0, &SELECT);
		sprintf(filePath, "%s%s", resSND, "enter.wav");
		FMOD_System_CreateSound(fmod_system, filePath, FMOD_CREATESAMPLE, 0, &ENTER);
		
		if (!SELECT || !ENTER) {
			fprintf(stderr, "<! Fatal> EasySDL: Missing sound ressources in %s please check for select.wav and enter.wav !\n", resSND);
			exit(0);
		}
	}
	
}

void SDL_unloadRessources() {
	
	if (BTN_OVER) SDL_FreeSurface(BTN_OVER);
	if (BTN_NOTOVER) SDL_FreeSurface(BTN_NOTOVER);
	if (FORM) SDL_FreeSurface(FORM);
	
	if (audio_loaded) {
		if (SELECT) FMOD_Sound_Release(SELECT);
		if (ENTER) FMOD_Sound_Release(ENTER);
	}
	
}

void SDL_unload() {
	
	SDL_unloadRessources();
	IMG_Quit();
	if (ttf_loaded) {
		TTF_CloseFont(ttf_police);
		ttf_police = NULL;
		TTF_Quit();
	}
	if (audio_loaded) {
		SDL_unloadallSound();
		FMOD_System_Close(fmod_system);
		FMOD_System_Release(fmod_system);
	}
	SDL_Quit();
	
	exit(0);

}

void SDL_generateFrame(t_context * context) {
	
	int i = 0;
	
	SDL_Rect positionFond, spritePos; 
	char saisie_content[100]; //Form ONLY
	
	if (context == NULL) return;
	
	context->contextSurface = SDL_CreateRGBSurface(0, context->height, context->width, 16, 0, 0, 0, 0);
	
	//Scan textures to Blit !
	for (i = 0; i < (context->nbImg); i++) {
		
		positionFond.x = context->contextImg[i].x;
		positionFond.y = context->contextImg[i].y;
		
		SDL_BlitSurface(context->contextImg[i].buffer, NULL, context->contextSurface, &positionFond);
		
	}
	
	//Scan for Rectangle to Blit
	
	for (i = 0; i < (context->nbRect); i++) {
		
		SDL_FillRect(context->contextSurface, &(context->contextRect[i].def), SDL_MapRGB(context->contextSurface->format, context->contextRect[i].color.r, context->contextRect[i].color.g, context->contextRect[i].color.b));
		
	}
	
	//Blit OBJ ONLY
	for (i = 0; i < (context->nbObj); i++) {
	
		switch (context->contextObj[i].type) {
		
			case BUTTON: //Simple btn
				
				positionFond.x = context->contextObj[i].x;
				positionFond.y = context->contextObj[i].y;
				
				if (context->contextObj[i].MouseOver == 1) {
					SDL_BlitSurface(BTN_OVER, NULL, context->contextSurface, &positionFond);
				}else{
					SDL_BlitSurface(BTN_NOTOVER, NULL, context->contextSurface, &positionFond);
				}
				
				switch (context->contextObj[i].align) {
					case ALIGN_CENTER:
						positionFond.x += ((BTN_OVER->w)/2)-((context->contextObj[i].buffer_title->w)/2);
						break;
					case ALIGN_LEFT:
						positionFond.x += 20;
						break;
					case ALIGN_RIGHT:
						positionFond.x += ((BTN_OVER->w)-(context->contextObj[i].buffer_title->w))-5;
						
						break;
				}
				
				
				positionFond.y += 5;
				
				SDL_BlitSurface(context->contextObj[i].buffer_title, NULL, context->contextSurface, &positionFond);
				
				break;
				
			case INPUT: //Form
				
				memset (saisie_content, 0, sizeof (saisie_content));
				
				if (context->contextObj[i].MouseOver == 1) {
					
					strcpy (saisie_content, context->contextObj[i].dest);
  					strcat (saisie_content,"|");
  					
				}else{
					
					strcpy (saisie_content, context->contextObj[i].dest);
					
				}
				
				positionFond.x = context->contextObj[i].x;
				positionFond.y = context->contextObj[i].y;
				
				SDL_BlitSurface(FORM, NULL, context->contextSurface, &positionFond);
				
				if (context->contextObj[i].buffer_content) {
					SDL_FreeSurface(context->contextObj[i].buffer_content);
					context->contextObj[i].buffer_content = NULL;
				}
				
				context->contextObj[i].buffer_content = TTF_RenderText_Blended(ttf_police, saisie_content, colorBlack);
				
				switch (context->contextObj[i].align) {
					case ALIGN_CENTER:
						positionFond.x += ((FORM->w)/2)-((context->contextObj[i].buffer_content->w)/2);
						break;
					case ALIGN_LEFT:
						positionFond.x += 20;
						break;
					case ALIGN_RIGHT:
						positionFond.x += 20;
						break;
				}
				
				positionFond.y = (context->contextObj[i].y)+5;
				SDL_BlitSurface(context->contextObj[i].buffer_content, NULL, context->contextSurface, &positionFond);
				
				positionFond.x = (context->contextObj[i].x)-55;
				positionFond.y = (context->contextObj[i].y)+5;
				SDL_BlitSurface(context->contextObj[i].buffer_title, NULL, context->contextSurface, &positionFond);
				
				break;
				
			default:
				break;
				
		}
		
	}
	
	//For each surface that correspond to text, blit !
	for (i = 0; i < (context->nbText); i++) {
		
		positionFond.x = context->contextText[i].x;
		positionFond.y = context->contextText[i].y;
		
		SDL_BlitSurface(context->contextText[i].buffer, NULL, context->contextSurface, &positionFond);
			
	}
	
		//Scan for active sprite..
	for (i = 0; i < (context->nbSprite); i++) {
		
		if (!(context->contextSprite[i].hide)) {
			
			//Animation .. Orientation
			spritePos.x = ((context->contextSprite[i].animation % ((context->contextSprite[i].buffer->w) / (context->contextSprite[i].sp_width))) * (context->contextSprite[i].sp_width))-(context->contextSprite[i].sp_width);
    		spritePos.y = ((context->contextSprite[i].position) * (context->contextSprite[i].sp_height))-(context->contextSprite[i].sp_height);
    		spritePos.w = context->contextSprite[i].sp_width;
    		spritePos.h = context->contextSprite[i].sp_height;
    		
    		positionFond.x = context->contextSprite[i].x;
			positionFond.y = context->contextSprite[i].y;
    		
    		SDL_BlitSurface(context->contextSprite[i].buffer, &spritePos, context->contextSurface, &positionFond );
		}
	}
	
	positionFond.w = 0;
	positionFond.h = 0;
	
	positionFond.x = context->x;
	positionFond.y = context->y;
	SDL_BlitSurface(context->contextSurface, NULL, screen, &positionFond);
	
	SDL_FreeSurface(context->contextSurface);
	
}

int SDL_generateMenu(char * backgroundPic, int nbEntries, char ** captions) {
	
	int i = 0, MouseOverObj = 0, MouseOverObjPrev = 0, firstFrame = 0;
	
	t_context * menu = SDL_newContext("Menu", 0, 0, 800, 600);
	
	for (i = 0; i < nbEntries; i++) {
		SDL_newObj(menu, NULL, BUTTON, captions[i], ALIGN_CENTER, NULL, NONE, 100, 100+(50*i));
	}
	
	SDL_newImage(menu, NULL, backgroundPic, 0, 0);
	
	while (1) {
		
		do {
			
			MouseOverObjPrev = MouseOverObj;
			SDL_UpdateEvents(&in);
			MouseOverObj = SDL_ismouseover(menu, BUTTON);
			if (MouseOverObj == -1) MouseOverObj = SDL_ismouseover(menu, INPUT);
			
			if (firstFrame == 0) { firstFrame = 1; break; }
			SDL_Delay(50);
			
		} while ((MouseOverObjPrev == MouseOverObj) && (!in.mousebuttons[SDL_BUTTON_LEFT]) && (in.quit != 1));
		
		if (MouseOverObjPrev != MouseOverObj) {		
			SDL_generateFrame(menu);
			SDL_Flip (screen);
			SDL_FreeSurface(screen);
		}
		
		if ((MouseOverObj != -1) && ((menu->contextObj[MouseOverObj].type) == 0)) {
			if (audio_loaded) FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, SELECT, 0, NULL);
		}
		
		//If user clic with left btn on object
		if ((in.mousebuttons[SDL_BUTTON_LEFT]) && (MouseOverObj != -1)) {
			
			if (audio_loaded) FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, ENTER, 0, NULL);
			in.mousebuttons[SDL_BUTTON_LEFT] = 0;
			SDL_freeContext(menu);
			
			return MouseOverObj;
		
		}else if((in.mousebuttons[SDL_BUTTON_LEFT])) { //Solve bug with SDL 2 when user clic on top bar (just for SDL_generateMenu)
		
			in.mousebuttons[SDL_BUTTON_LEFT] = 0;
			
		}
		
		if (in.quit) {
			SDL_freeContext(menu);
			exit(0);
		}
		
	}
	
}

int SDL_generate(t_context * context) {
	
	int MouseOverObj = -1, MouseOverObjPrev = 0, firstFrame = 0, forceFrame = 0;
	int uniqueFrame = 0;
	
	if (context == NULL) return -1;
	
	if (!(context->nbObj)) {
		uniqueFrame = 1;
	}
	
	while (1) {
		
		do {
			
			MouseOverObjPrev = MouseOverObj;
			SDL_UpdateEvents(&in);
			MouseOverObj = SDL_ismouseover(context, BUTTON);
			if (MouseOverObj == -1) MouseOverObj = SDL_ismouseover(context, INPUT);
			
			if (!firstFrame) { firstFrame = 1; break; }
			SDL_Delay(50);
			
		} while ((MouseOverObjPrev == MouseOverObj) && (!in.mousebuttons[SDL_BUTTON_LEFT]) && (buffer_deliver == 1) && (in.quit != 1) && (uniqueFrame != 1));
		
		if (buffer_deliver == 0) {
			SDL_captureInput(context, MouseOverObj);
			forceFrame = 1;
		}
		
		if ((MouseOverObjPrev != MouseOverObj) || (forceFrame == 1) || (uniqueFrame == 1) || (firstFrame == 1)) {		
			SDL_generateFrame(context);
			SDL_Flip (screen);
			SDL_FreeSurface(screen);
			forceFrame = 0;
			
			if (uniqueFrame == 1) {
				if (in.quit) {
					SDL_freeContext(context);
					exit(0);
				}
				SDL_Delay(DELAY_EACH_FRAME);
				return 0;
			}
			if (firstFrame == 1) firstFrame = 2;
		}
		
		if ((MouseOverObj != -1) && ((context->contextObj[MouseOverObj].type) == 0)) {
			if (audio_loaded) FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, SELECT, 0, NULL);
		}
		
		//If user clic (left btn)
		if ((in.mousebuttons[SDL_BUTTON_LEFT]) && (MouseOverObj != -1) && ((context->contextObj[MouseOverObj].type) == 0) ) {
			
			if (audio_loaded) FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, ENTER, 0, NULL);
			in.mousebuttons[SDL_BUTTON_LEFT] = 0;

			return MouseOverObj;
		
		}
		
		if (in.quit == 1) {
			SDL_freeContext(context);
			exit(0);
		}
		
	}
	
}
