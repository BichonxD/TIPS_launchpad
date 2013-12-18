
#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/inc/fmod.h"
#include "../lib/inc/fmod_errors.h"
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "findus.h"

#define NB_TOUCHES 400
#define TAILLE_ARGV 4
#define TAILLE_STRING 15

typedef struct touche
{
	SDLKey key;
	FMOD_SOUND* son;
} Touche;

int init_touches(FMOD_SYSTEM*, Touche*);
void libere_touches(Touche*);
int isWAV(char* string);

#endif
