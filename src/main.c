
#include "main.h"

int main(int argc, char *argv[])
{
	int continuer = 1;
	float volume = 1.0;
	Touche tab[NB_TOUCHES];
	SDL_Surface *ecran = NULL, *fde = NULL;
	SDL_Event event;
	SDL_Rect positionFond;
	FMOD_SYSTEM *system;
	FMOD_CHANNELGROUP* channel = null;
	
	positionFond.x = 0;
	positionFond.y = 0;
	
	/*SONS*/
	/* Création et initialisation d'un objet système pour le FMOD*/
	FMOD_System_Create(&system);
	FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
	
	/* Chargement des sons */
	if(init_touches(system, tab))
		return EXIT_FAILURE;
	
	/*SDL*/
	/* Initialisation de la SDL */
	SDL_Init(SDL_INIT_VIDEO);
	
	ecran = SDL_SetVideoMode(283, 200, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Launchpad de la TIPS", NULL);
	
	fde = IMG_Load("../files/fde.png");
	SDL_WM_SetIcon(fde, NULL);
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_BlitSurface(fde, NULL, ecran, &positionFond);
	SDL_Flip(ecran);
	
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_MOUSEBUTTONDOWN: /* Si clique de la souris */
				/* Non utile pour le moment*/
				break;
			case SDL_KEYDOWN: /* Si appuie sur une touche */
				
				/* Esc pour quitter */
				if(event.key.keysym.sym == SDLK_ESCAPE)
					continuer = 0;
				/* UP pour monter le son */
				else if(event.key.keysym.sym == SDLK_UP)
				{
					FMOD_System_GetMasterChannelGroup(system, &channel);
					volume += 0.1;
					FMOD_ChannelGroup_SetVolume(channel, volume);
				}
				/* DOWN pour baisser le son */
				else if(event.key.keysym.sym == SDLK_DOWN)
				{
					FMOD_System_GetMasterChannelGroup(system, &channel);
					volume -= 0.1;
					FMOD_ChannelGroup_SetVolume(channel, volume);
				}
				/* LEFT ou RIGHT pour stopper la musique */
				else if((event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_RIGHT))
				{
					FMOD_System_GetMasterChannelGroup(system, &channel);
					FMOD_ChannelGroup_Stop(channel);
				}
				else if(tab[event.key.keysym.sym].son != NULL)
				{
					FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, tab[event.key.keysym.sym].son, 0, NULL);
				}
				break;
		}
	}

	/* On ferme la SDL */
	SDL_FreeSurface(fde);
	SDL_Quit();

	/* On libère le son et on ferme et libère l'objet système */
	libere_touches(tab);
	FMOD_System_Close(system);
	FMOD_System_Release(system);
	
	return EXIT_SUCCESS;
}

int init_touches(FMOD_SYSTEM* system, Touche* tab)
{
	int i, j;
	Retour* ret = null;
	FMOD_RESULT resultat;
	struct stat statistiques;
	
	/*Initialisation de la recherche des fichiers*/
	char** argv = null;
	argv = (char**) calloc(TAILLE_ARGV, sizeof(char*));
	for(i = 0; i < TAILLE_ARGV; i++)
	{
		argv[i] = (char*) calloc(TAILLE_STRING, sizeof(char));
	}
	strcpy(argv[0], "./findus");
	strcpy(argv[1], "../files/");
	strcpy(argv[2], "-name");
	
	for(i = 0; i < NB_TOUCHES; i++)
	{
		/*En fonction de où on se trouve dans le tableau on met le son qu'on veut */
		switch(i)
		{
			case SDLK_BACKSPACE:
				strcpy(argv[3], "BACKSPACE_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_TAB:
				strcpy(argv[3], "TAB_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_CLEAR:
				strcpy(argv[3], "CLEAR_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RETURN:
				strcpy(argv[3], "RETURN_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_PAUSE:
				strcpy(argv[3], "PAUSE_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_SPACE:
				strcpy(argv[3], "SPACE_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_EXCLAIM:
				strcpy(argv[3], "!_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_QUOTEDBL:
				strcpy(argv[3], "\"_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_HASH:
				strcpy(argv[3], "#_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_DOLLAR:
				strcpy(argv[3], "$_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_AMPERSAND:
				strcpy(argv[3], "&_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_QUOTE:
				strcpy(argv[3], "'_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LEFTPAREN:
				strcpy(argv[3], "(_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RIGHTPAREN:
				strcpy(argv[3], ")_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_ASTERISK:
				strcpy(argv[3], "ETOILE_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_PLUS:
				strcpy(argv[3], "+_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_COMMA:
				strcpy(argv[3], ",_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_MINUS:
				strcpy(argv[3], "-_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_PERIOD:
				strcpy(argv[3], "._*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_SLASH:
				strcpy(argv[3], "SLASH_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_COLON:
				strcpy(argv[3], ":_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_SEMICOLON:
				strcpy(argv[3], ";_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LESS:
				strcpy(argv[3], "<_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_EQUALS:
				strcpy(argv[3], "=_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_GREATER:
				strcpy(argv[3], ">_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_QUESTION:
				strcpy(argv[3], "QUESTION_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_AT:
				strcpy(argv[3], "@_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LEFTBRACKET:
				strcpy(argv[3], "[_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_BACKSLASH:
				strcpy(argv[3], "BACKSLASH_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RIGHTBRACKET:
				strcpy(argv[3], "]_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_CARET:
				strcpy(argv[3], "^_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_UNDERSCORE:
				strcpy(argv[3], "__*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_BACKQUOTE:
				strcpy(argv[3], "`_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_DELETE:
				strcpy(argv[3], "DEL_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP0:
				strcpy(argv[3], "KP0_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP1:
				strcpy(argv[3], "KP1_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP2:
				strcpy(argv[3], "KP2_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP3:
				strcpy(argv[3], "KP3_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP4:
				strcpy(argv[3], "KP4_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP5:
				strcpy(argv[3], "KP5_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP6:
				strcpy(argv[3], "KP6_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP7:
				strcpy(argv[3], "KP7_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP8:
				strcpy(argv[3], "KP8_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP9:
				strcpy(argv[3], "KP9_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_PERIOD:
				strcpy(argv[3], "KP._*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_DIVIDE:
				strcpy(argv[3], "KP/_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_MULTIPLY:
				strcpy(argv[3], "KPETOILE_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_MINUS:
				strcpy(argv[3], "KP-_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_PLUS:
				strcpy(argv[3], "KP+_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_ENTER:
				strcpy(argv[3], "KPENTER_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_KP_EQUALS:
				strcpy(argv[3], "KP=_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_INSERT:
				strcpy(argv[3], "INSERT_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_HOME:
				strcpy(argv[3], "HOME_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_END:
				strcpy(argv[3], "END_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_PAGEUP:
				strcpy(argv[3], "PAGEUP_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_PAGEDOWN:
				strcpy(argv[3], "PAGEDOWN_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F1:
				strcpy(argv[3], "F1_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F2:
				strcpy(argv[3], "F2_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F3:
				strcpy(argv[3], "F3_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F4:
				strcpy(argv[3], "F4_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F5:
				strcpy(argv[3], "F5_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F6:
				strcpy(argv[3], "F6_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F7:
				strcpy(argv[3], "F7_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F8:
				strcpy(argv[3], "F8_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F9:
				strcpy(argv[3], "F9_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F10:
				strcpy(argv[3], "F10_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F11:
				strcpy(argv[3], "F11_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F12:
				strcpy(argv[3], "F12_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F13:
				strcpy(argv[3], "F13_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F14:
				strcpy(argv[3], "F14_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_F15:
				strcpy(argv[3], "F15_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_NUMLOCK:
				strcpy(argv[3], "NUMLOCK_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_CAPSLOCK:
				strcpy(argv[3], "CAPSLOCK_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_SCROLLOCK:
				strcpy(argv[3], "SCROLLOCK_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RSHIFT:
				strcpy(argv[3], "RSHIFT_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LSHIFT:
				strcpy(argv[3], "LSHIFT_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RCTRL:
				strcpy(argv[3], "RCTRL_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LCTRL:
				strcpy(argv[3], "LCTRL_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RALT:
				strcpy(argv[3], "RALT_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LALT:
				strcpy(argv[3], "LALT_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RMETA:
				strcpy(argv[3], "RMETA_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LMETA:
				strcpy(argv[3], "LMETA_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_RSUPER:
				strcpy(argv[3], "RSUPER_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_LSUPER:
				strcpy(argv[3], "LSUPER_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_MODE:
				strcpy(argv[3], "MODE_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_HELP:
				strcpy(argv[3], "HELP_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_PRINT:
				strcpy(argv[3], "PRINT_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_SYSREQ:
				strcpy(argv[3], "SYSREQ_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_BREAK:
				strcpy(argv[3], "BREAK_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			case SDLK_MENU:
				strcpy(argv[3], "MENU_*");
				ret = find(TAILLE_ARGV, argv);
				break;
			default:
				ret = NULL;
				break;
		}
		
		/* Les chifres */
		if(i >= '0' && i <= '9')
		{
			argv[3][0] =  i ;
			strcat(argv[3], "_*");
			ret = find(TAILLE_ARGV, argv);
		}
		/* Les lettres */
		if(i >= 'a' && i <= 'z')
		{
			argv[3][0] =  i ;
			strcat(argv[3], "_*");
			ret = find(TAILLE_ARGV, argv);
		}
		
		for(j = 0; j < TAILLE_STRING; j++)
		{
			argv[3][j] = 0;
		}
		
		/* Si la chanson existe */
		if(ret == NULL)
		{
			tab[i].son = NULL;
		}
		else if(*(ret->taille) == 1)
		{
			stat((*(ret->retour))[0], &statistiques);
			/* Chargement du son et vérification du chargement */
			/* SAMPLES = WAV ou taille_fichier < 1 Mo (à implémenter pour l'instant ce n'est que du WAV)*/
			if((isWAV((*(ret->retour))[0])) || ((long) (statistiques.st_size) < 1024*1024))
			{
				resultat = FMOD_System_CreateSound(system, (*(ret->retour))[0], FMOD_CREATESAMPLE, 0, &(tab[i].son));
				if(resultat != FMOD_OK)
				{
					fprintf(stderr, "Impossible de lire %s\n", (*(ret->retour))[0]);
					return 1;
				}
			}
			/* Le reste est musique */
			else
			{
				resultat = FMOD_System_CreateSound(system, (*(ret->retour))[0], FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &(tab[i].son));
				if(resultat != FMOD_OK)
				{
					fprintf(stderr, "Impossible de lire %s\n", (*(ret->retour))[0]);
					return 1;
				}
			}
			
			/* On libere la structure utilisée pour le résultat de findus */
			for(j = 0; j < *(ret->taille); j++)
			{
				free((*(ret->retour))[j]);
			}
			free(*(ret->retour));
			free(ret->retour);
			free(ret->taille);
			free(ret);
		}
		/* Si plus d'une chanson existe alors erreur */
		else if(*(ret->taille) > 1)
		{
			fprintf(stderr, "ERREUR : Il existe plusieurs chansons commencant par le meme identificateur.\n");
			printf("Voici la liste :\n");
			for(j = 0; j < *(ret->taille); j++)
			{
				printf("%s\n", (*(ret->retour))[j]);
			}
			return 1;
		}
		/* Si cette touche est vide */
		else
		{
			tab[i].son = NULL;
			/* On libere la structure utilisée pour le résultat de findus */
			free(*(ret->retour));
			free(ret->retour);
			free(ret->taille);
			free(ret);
		}
	}
	
	/* On libere le tableau de chaine de caracteres argv */
	for(i = 0; i < TAILLE_ARGV; i++)
	{
		free(argv[i]);
	}
	free(argv);
	
	return 0;
}

void libere_touches(Touche* tab)
{
	int i;
	
	for(i = 0; i < NB_TOUCHES; i++)
	{
		FMOD_Sound_Release(tab[i].son);
	}
}

int isWAV(char* string)
{
	if(string[strlen(string)-4] == '.')
	{
		if((string[strlen(string)-3] == 'W') || (string[strlen(string)-3] == 'w'))
		{
			if((string[strlen(string)-2] == 'A') || (string[strlen(string)-2] == 'a'))
			{
				if((string[strlen(string)-1] == 'V') || (string[strlen(string)-1] == 'v'))
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
}
