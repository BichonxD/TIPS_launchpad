
#include "findus.h"

Retour* find(int argc, char ** argv)
{
	/*Construction de la structure de paramètres*/
	int i, j;
	int echange;
	char temp[512];
	parametres * params = null;
	params = lecture_param(argc, argv);
	Retour* ret = null;
	ret = (Retour*) calloc(1, sizeof(Retour));
	
	char *** retour = calloc(1, sizeof(char **));
	int * taille = calloc(1, sizeof(int));
	if(!retour)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	if(!taille)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	
	findFiles(params, getCheminDepart(params), 0, retour, taille);
	
	if(params && params->sort)
	{
		do
		{
			echange = 0;
			for(j = 0; j < *taille - 1; j++)
			{
				if(strcmp((*retour)[j], (*retour)[j+1]) > 0)
				{
					strcpy(temp, (*retour)[j]);
					(*retour)[j] = realloc((*retour)[j], strlen((*retour)[j+1])+1);
					strcpy((*retour)[j], (*retour)[j+1]);
					(*retour)[j+1] = realloc((*retour)[j+1], strlen(temp)+1);
					strcpy((*retour)[j+1], temp);
					echange = 1;
				}
			}
		} while(echange);
	}
	
	for(i=0; params->cmds[i]; i++)
	{
		detruire_commande(params->cmds[i]);
	}
	free(params->cmds);
	free(params->path);
	free(params);
	
	ret->retour = retour;
	ret->taille = taille;
	
	return ret;
}

void detruire_commande(commande * cmd)
{
	if(!cmd) return;
	
	int j;
	for(j=0; j<cmd->nb_args; j++)
	{
		free(cmd->arguments[j]);
	}
	free(cmd->arguments);
	detruire_commande(cmd->arg1);
	detruire_commande(cmd->arg2);
	free(cmd);
}

char * mode2string(mode_t m)
{
	int i, j;
	char * perm_fichier = calloc(6, sizeof(char));
	sprintf(perm_fichier, "%04o\n", m & 0777);
	
	char * retour = calloc(11, sizeof(char));
	if(S_ISREG(m))
	{
		retour[0] = '-';
	}
	else if(S_ISDIR(m))
	{
		retour[0] = 'd';
	}
	else if(S_ISCHR(m))
	{
		retour[0] = 'c';
	}
	else if(S_ISBLK(m))
	{
		retour[0] = 'b';
	}
	else if(S_ISFIFO(m))
	{
		retour[0] = 'f';
	}
	else if(S_ISLNK(m))
	{
		retour[0] = 'l';
	}
	else if(S_ISSOCK(m))
	{
		retour[0] = 's';
	}
	else
	{
		retour[0] = '-';
	}
	
	
	j=1;
	//Pas de permission particulière
	for(i=1;i<4;i++)
	{
		switch(perm_fichier[i])
		{
			case '0' :
				retour[j] = '-';
				retour[j+1] = '-';
				retour[j+2] = '-';
			break;
			case '1' :
				retour[j] = '-';
				retour[j+1] = '-';
				retour[j+2] = 'x';
			break;
			case '2' :
				retour[j] = '-';
				retour[j+1] = 'w';
				retour[j+2] = '-';
			break;
			case '3' :
				retour[j] = '-';
				retour[j+1] = 'w';
				retour[j+2] = 'x';
			break;
			case '4' :
				retour[j] = 'r';
				retour[j+1] = '-';
				retour[j+2] = '-';
			break;
			case '5' :
				retour[j] = 'r';
				retour[j+1] = '-';
				retour[j+2] = 'x';
			break;
			case '6' :
				retour[j] = 'r';
				retour[j+1] = 'w';
				retour[j+2] = '-';
			break;
			case '7' :
				retour[j] = 'r';
				retour[j+1] = 'w';
				retour[j+2] = 'x';
			break;
			default :
				retour[j] = '-';
				retour[j+1] = '-';
				retour[j+2] = '-';
			break;
		}
		j+=3;
	}
	
	if(*perm_fichier != '0')
	{
		switch(perm_fichier[0])
		{
			case '0' :
			break;
			case '1' :
				retour[9] = 'T';
			break;
			case '2' :
				retour[6] = 's';
			break;
			case '3' :
				retour[6] = 's';
				retour[9] = 'T';
			break;
			case '4' :
				retour[3] = 's';
			break;
			case '5' :
				retour[3] = 's';
				retour[9] = 'T';
			break;
			case '6' :
				retour[3] = 's';
				retour[6] = 's';
			break;
			case '7' :
				retour[3] = 's';
				retour[6] = 's';
				retour[9] = 'T';
			break;
			default :
			break;
		}
	}
	
	free(perm_fichier);
	
	return retour;
}

void findFiles(parametres * params, char * chemin, int niveau, char *** retour, int * taille)
{
	if(chemin != null)
	{
		char chemin_complet[512];
		struct stat st;
		DIR * dir = opendir(chemin);
		if(!dir)
		{
			fprintf(stderr, "Erreur : %s\n", chemin);
			return;
		}
		struct dirent * temp = readdir(dir);
		
		/*Afficher le dossier racine du find*/
		if(!niveau && (params && ((params->mindepth == -1) || (params->mindepth <= niveau))))
		{
			lstat(chemin, &st);
			/*Savoir si on doit afficher le '.'*/
			do
			{
				temp = readdir(dir);
			} while(temp && strcmp(temp->d_name, "."));
			
			if(temp && (!params || execute_commande(params->cmds, temp, &st, chemin)))
			{
				if(!strcmp(chemin, "./"))
				{
					*taille = *taille + 1;
					*retour = realloc(*retour, *taille*sizeof(char *));
					(*retour)[*taille-1] = calloc(2, sizeof(char));
					strcpy((*retour)[*taille-1], ".");
				}
				else if(!niveau)
				{
					*taille = *taille + 1;
					*retour = realloc(*retour, *taille*sizeof(char *));
					(*retour)[*taille-1] = calloc(strlen(chemin) + 1, sizeof(char));
					sprintf((*retour)[*taille-1], "%s", chemin);
				}
			}
			
			rewinddir(dir);
			temp = null;
		}
		
		while((temp = readdir(dir)))
 		{
 			sprintf(chemin_complet, "%s%s", chemin, temp->d_name);
 			lstat(chemin_complet, &st);
 			
			/*Afficher le nom*/
			if((((strcmp(temp->d_name, ".")) && strcmp(temp->d_name, "..")) && (!params || execute_commande(params->cmds, temp, &st, chemin_complet))) && (params && ((params->mindepth == -1) || (params->mindepth <= niveau))))
			{
				//printf("%s%s\n", chemin, temp->d_name);
				*taille = *taille + 1;
				*retour = realloc(*retour, (*taille) * sizeof(char *));
				(*retour)[(*taille)-1] = calloc(strlen(chemin) + strlen(temp->d_name) + 1, sizeof(char));
				sprintf((*retour)[*taille-1], "%s%s", chemin, temp->d_name);
			}
			
			/*Si c'est un dossier, on entre dedans et on rappelle la fonction*/
			if((!params || (params && !params->prune)) && 4 == temp->d_type)
			{
				/*Si le dossier n'est pas le dossier courant ni le dossier parent*/
				if(strcmp(temp->d_name, ".") && strcmp(temp->d_name, ".."))
				{
					if(!params || params->maxdepth == -1  || params->maxdepth >= niveau + 1)
					{
						/*Ouvrir le dossier et mettre a jour le nom du chemin*/
						char * tmp = calloc(strlen(chemin) + strlen(temp->d_name) + 2, sizeof(char));
						sprintf(tmp, "%s%s/", chemin, temp->d_name);
					
						/*Appel recursif sur le dossier enfant*/
						findFiles(params, tmp, niveau + 1, retour, taille);
						free(tmp);
					 }
				}
			}
		}
		closedir(dir);
		//free(chemin);
	}
}

char * getCheminDepart(parametres * params)
{
	DIR * dir = NULL;
	if(!params || !params->path || !(dir = opendir(params->path)))
		return "./";
	else
	{
		/*char * tmp = calloc(strlen(params->path) + 1, sizeof(char));
		strcpy(tmp, params->path);*/
		if(dir) closedir(dir);
		return params->path;
	}
}
