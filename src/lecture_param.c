
#include "findus.h"

parametres * lecture_param(int argc, char * argv[])
{
	int i;
	/*Le tableau result contiendra le résultat de la conversion des paramètres en notation polonaise inversée.*/
	commande ** result = NULL;
	int nb_elts_tab;
	/*Le tableau de commande pile nous tiendra lieu de pile pour la conversion en notation polonaise inversée*/
	commande ** pile = NULL;
	int nb_elts_pile = 0;
	parametres * p = NULL;
	int nb_parenthese_o = 0, nb_parenthese_f = 0;
	/*Cet int sert à dires s'il y a deux prédicats à la suite (non séparés par un opérateur ni par une parenthèse).
	Dans ce cas il faut ajouté un -and entre les deux prédicats et l'argument boolAnd sera mis à 1 pour ça.*/
	int predicat = 0, boolAnd = 0, operateur = 0;
	/*Sert à dire si l'utilisateur a mal placé l'appel a une fonction*/
	int fonction_mal_place = 0, f_presente = 0, op_present = 0;
	
	/*Il y aura au plus argc commande donc nous créons un tableau de taille argc+1*/
	if((result = (commande**) calloc(argc+1, sizeof(commande*))) == NULL)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	/*Il y aura au plus argc commande donc nous créons un tableau de taille argc+1*/
	if((pile = (commande**) calloc(argc+1, sizeof(commande*))) == NULL)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	
	if((p = (parametres *) calloc(1, sizeof(parametres))) == NULL)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	
	p->cmds = result;
	p->mindepth = -1;
	p->maxdepth = -1;
	p->sort = 0;
	p->prune = 0;
	p->ls = 0;
	p->action = 0;
	p->path = NULL;
	
	/*On initialise les tableaux a NULL*/
	for(i = 0; i < argc+1; result[i] = NULL, pile[i] = NULL, i++);
	nb_elts_tab = 0;
	nb_elts_pile = 0;
	
	/*On parcourt tous les arguments donnes a findus*/
	for(i = 1; i < argc; i++)
	{
		/*Si l'argument est une fonction*/
		if(argv[i][0] == '-')
		{
			/*Trouve la fonction/commande adaptee*/
			
			/*Si c'est l'une des suivantes alors nous sommes dans le cas d'un prédicat. Il sera lors directement ajouté à la sortie.
			C'est à dire à result.
			Chaque prédicat récupère lui-même ces propres parametres et exit() si ils n'y sont pas.*/
			if(strcmp(argv[i], "-true") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction true*/
					result[nb_elts_tab]->fonction = (void *)true;
					/*Les arguments sont mis a NULL. Il n'y en a pas.*/
					result[nb_elts_tab]->arguments = NULL;
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					result[nb_elts_tab]->nb_args = 0;
					
					/*On incrémente pour signaler qu'il y a un élément de plus dans la pile.*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-false") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction false*/
					result[nb_elts_tab]->fonction = (void *)false;
					/*Les arguments sont mis a NULL. Il n'y en a pas.*/
					result[nb_elts_tab]->arguments = NULL;
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					result[nb_elts_tab]->nb_args = 0;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-name") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres name*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -name »");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction name*/
					result[nb_elts_tab]->fonction = (void *)name;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-type") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres type*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -type »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction type*/
					result[nb_elts_tab]->fonction = (void *)type;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					if(strlen(argv[i]) != 1)
					{
						perror("findUs: Les arguments pour -type devraient contenir seulement une lettre");
						exit(EXIT_FAILURE);
					}
					
					if(charTypeToInt(argv[i][0]) == DT_UNKNOWN)
					{
						perror("findUs: Argument inconnu pour -type");
						exit(EXIT_FAILURE);
					}
					
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-uid") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres uid*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -uid »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction uid*/
					result[nb_elts_tab]->fonction = (void *)uid;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-gid") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres gid*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -gid »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction gid*/
					result[nb_elts_tab]->fonction = (void *)gid;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-user") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres user*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -user »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction user*/
					result[nb_elts_tab]->fonction = (void *)user;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-group") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres group*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -group »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction group*/
					result[nb_elts_tab]->fonction = (void *)group;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-atime") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres atime*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -atime »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction atime*/
					result[nb_elts_tab]->fonction = (void *)atime;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					if(!atoi(argv[i]))
					{
						perror("findUs: Paramètre manquant pour « -atime »");
						exit(EXIT_FAILURE);
					}
					
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-ctime") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres ctime*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -ctime »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction ctime*/
					result[nb_elts_tab]->fonction = (void *)ctime_;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					if(!atoi(argv[i]))
					{
						perror("findUs: Paramètre manquant pour « -ctime »");
						exit(EXIT_FAILURE);
					}
					
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-mtime") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres mtime*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -mtime »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction mtime*/
					result[nb_elts_tab]->fonction = (void *)mtime;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					if(!atoi(argv[i]))
					{
						perror("findUs: Paramètre manquant pour « -mtime »");
						exit(EXIT_FAILURE);
					}
					
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-perm") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres perm*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -perm »");
						exit(EXIT_FAILURE);
					}
					/*On alloue la fonction perm*/
					result[nb_elts_tab]->fonction = (void *)perm;
					/*On alloue ce qu'il faut pour les paramètres*/
					if((result[nb_elts_tab]->arguments = (char**) calloc(1, sizeof(char*))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					if((result[nb_elts_tab]->arguments[0] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					/*On copie le parametre*/
					strcpy(result[nb_elts_tab]->arguments[0], argv[i]);
					if(!(*argv[i] >= '0' && *argv[i] <= '9'))
					{
						//Masque
						if(!(*argv[i] == '/' || *argv[i] == '-'))
						{
							perror("findUs: -perm : mode non pris en charge");
							exit(EXIT_FAILURE);
						}
					}
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					/*Il n'y a qu'un seul parametre*/
					result[nb_elts_tab]->nb_args = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-print") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction print*/
					result[nb_elts_tab]->fonction = (void *)print;
					/*Les arguments sont mis a NULL. Il n'y en a pas.*/
					result[nb_elts_tab]->arguments = NULL;
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					result[nb_elts_tab]->nb_args = 0;
					
					/*On signale qu'une action a été commandée*/
					p->action = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-ls") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction ls*/
					result[nb_elts_tab]->fonction = (void *)ls;
					/*Les arguments sont mis a NULL. Il n'y en a pas.*/
					result[nb_elts_tab]->arguments = NULL;
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					result[nb_elts_tab]->nb_args = 0;
					p->ls = 1;
					
					/*On signale qu'une action a été commandée*/
					p->action = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-exec") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On vérifie qu'il existe bien un parametre apres exec. La fin du parametre est déterminée par un ;*/
					i++;
					if(i >= argc)
					{
						perror("findUs: Paramètre manquant pour « -exec »");
						exit(EXIT_FAILURE);
					}
					/*Si le premier argument est un ';'*/
					if(i == 0)
					{
						perror("findUs: Paramètre non valide « ; » pour « -exec »");
						exit(EXIT_FAILURE);
					}
					result[nb_elts_tab]->nb_args = 0;
					
					/*On alloue la fonction exec*/
					result[nb_elts_tab]->fonction = (void *)exec;
					
					result[nb_elts_tab]->arguments = null;
					
					while(i<argc && argv[i][0] != ';')
					{
						if((result[nb_elts_tab]->arguments = (char**) realloc(result[nb_elts_tab]->arguments,++result[nb_elts_tab]->nb_args*sizeof(char*))) == NULL)
						{
							perror("realloc");
							exit(EXIT_FAILURE);
						}
						if((result[nb_elts_tab]->arguments[result[nb_elts_tab]->nb_args-1] = (char*) calloc(strlen(argv[i]) + 1, sizeof(char))) == NULL)
						{
							perror("calloc");
							exit(EXIT_FAILURE);
						}
						/*On copie le parametre*/
						strcpy(result[nb_elts_tab]->arguments[result[nb_elts_tab]->nb_args-1], argv[i]);
						
						i++;
					}
					
					/*Le reste est mis a NULL*/
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					
					/*On signale qu'une action a été commandée*/
					p->action = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			else if(strcmp(argv[i], "-prune") == 0)
			{
				/*Si l'arguement précédent était un prédicat il faut ajouter un -a. Nous reviendrons ici après cet ajout d'où le else*/
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*On alloue une nouvelle commande dans result*/
					if((result[nb_elts_tab] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction prune*/
					result[nb_elts_tab]->fonction = (void *)prune;
					/*Les arguments sont mis a NULL. Il n'y en a pas.*/
					result[nb_elts_tab]->arguments = NULL;
					result[nb_elts_tab]->arg1 = NULL;
					result[nb_elts_tab]->arg2 = NULL;
					result[nb_elts_tab]->nb_args = 0;
					p->prune = 1;
					
					/*On incrémente pour passer à la commande suivante*/
					nb_elts_tab++;
					/*On signale que nous venons de lire un prédicat*/
					predicat = 1;
					operateur = 0;
					/*Si c'est le premier prédicat lu on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			
			/*A partir d'ici ce ne sont plus des prédicats mais des fonctions.
			Si elles ont été placées après des prédicats nous donnons un avertissement à l'utilisateur.
			Nous effectuons les actions impliquées par ces fonctions mais ne les nettons pas le tableau de résultat.
			Elles n'apparraissent pas dans la notation polonaise inversée.*/
			else if(strcmp(argv[i], "-mindepth") == 0)
			{
				/*Si un prédicat a déjà été lu aors la fonction est mal placée. On le signale à l'utilisateur.*/
				if(fonction_mal_place)
				{
					perror("findUS : ERREUR : vous avez spécifié l'option -mindepth après un argument qui n'est pas une option mais les options sont positionnelles (-mindepth affecte les tests spécifiés avant aussi bien qu'après). SVP spécifiez des options avant les autres arguments.\n");
					exit(EXIT_FAILURE);
				}
				
				i++;
				if(i >= argc)
				{
					perror("findUs: Paramètre manquant pour « -mindepth »");
					exit(EXIT_FAILURE);
				}
				p->mindepth = atoi(argv[i]);
				/*On signale que nous avons lu une fonction*/
				f_presente = 1;
			}
			else if(strcmp(argv[i], "-maxdepth") == 0)
			{
				/*Si un prédicat a déjà été lu aors la fonction est mal placée. On le signale à l'utilisateur.*/
				if(fonction_mal_place)
				{
					printf("findUS : ERREUR : vous avez spécifié l'option -maxdepth après un argument qui n'est pas une option mais les options sont positionnelles (-maxdepth affecte les tests spécifiés avant aussi bien qu'après). SVP spécifiez des options avant les autres arguments.\n");
					exit(EXIT_FAILURE);
				}
				
				i++;
				if(i >= argc)
				{
					perror("findUs: Paramètre manquant pour « -maxdepth »");
					exit(EXIT_FAILURE);
				}
				p->maxdepth = atoi(argv[i]);
				/*On signale que nous avons lu une fonction*/
				f_presente = 1;
			}
			else if(strcmp(argv[i], "-sort") == 0)
			{
				/*Si un prédicat a déjà été lu aors la fonction est mal placée. On le signale à l'utilisateur.*/
				if(fonction_mal_place)
				{
					printf("findUS : ERREUR : vous avez spécifié l'option -sort après un argument qui n'est pas une option mais les options sont positionnelles (-sort affecte les tests spécifiés avant aussi bien qu'après). SVP spécifiez des options avant les autres arguments.\n");
					exit(EXIT_FAILURE);
				}
				
				p->sort = 1;
				/*On signale que nous avons lu une fonction*/
				f_presente = 1;
			}
			
			/*A partir d'ici nous n'avons plus que des opérateurs.
			Nous appliquons l'algorithme suivant :
			Soit l'opérateur o1 en cours
			
			tant qu’il y a un opérateur o2 dont la priorité est supérieur ou égale à celle d'o1 sur le haut de la pile
				on retire o2 de la pile pour le mettre dans la sortie
			On met o1 sur la pile.
			
			Cette algorithme est valable car le seul opérateur unaire est -not et c'est celui le plus prioritaire.
			*/
			
			/*Voici la liste de priorité des opérateurs dans l'ordre décroissant (où e est une expression) :
			* -not e
			* ! e
			* e -a e
			* e e
			* e -o e
			* e , e
			*/
			/*NB : les parenthèses dans la pile sont représentées par un pointeur NULL*/
			else if(strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "-and") == 0)
			{
				/*Si il y a eu un opérateur avant alors c'est qu'il y a erreur, cet opérateur est binaire*/
				if(operateur)
				{
					perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
					exit(EXIT_FAILURE);
				}
				
				/*Seuls -not et -a lui-même ont une priorité supérieur ou égale à -a*/
				while(nb_elts_pile != 0 && pile[nb_elts_pile-1] != NULL && (pile[nb_elts_pile-1]->fonction == (void*)not || pile[nb_elts_pile-1]->fonction == (void*)a))
				{
					/*On met pile[nb_elts_pile] dans le resultat*/
					result[nb_elts_tab] = pile[nb_elts_pile-1];
					pile[nb_elts_pile-1] = NULL;
					nb_elts_pile--;
					nb_elts_tab++;
				}
				
				/*On met un nouveau -a dans la pile*/
				if((pile[nb_elts_pile] = (commande*) calloc(1, sizeof(commande))) == NULL)
				{
					perror("calloc");
					exit(EXIT_FAILURE);
				}
				
				/*On lie à la fonction a*/
				pile[nb_elts_pile]->fonction = (void *)a;
				/*Les arguments sont mis a NULL. Ils seront mis en place lors de l'interprétation de la notation polonaise inversée.*/
				pile[nb_elts_pile]->arguments = NULL;
				pile[nb_elts_pile]->arg1 = NULL;
				pile[nb_elts_pile]->arg2 = NULL;
				pile[nb_elts_pile]->nb_args = 0;
				
				nb_elts_pile++;
				predicat = 0;
				operateur = 1;
			}
			else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-or") == 0)
			{
				/*Si il y a eu un opérateur avant alors c'est qu'il y a erreur, cet opérateur est binaire*/
				if(operateur)
				{
					perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
					exit(EXIT_FAILURE);
				}
				
				/*Seuls -not, -a et -o ont une priorité supérieur ou égale à -o*/
				while(nb_elts_pile != 0 && pile[nb_elts_pile-1] != NULL && (pile[nb_elts_pile-1]->fonction == (void*)not || pile[nb_elts_pile-1]->fonction == (void*)a || pile[nb_elts_pile-1]->fonction == (void*)o))
				{
					/*On met pile[nb_elts_pile] dans le resultat*/
					result[nb_elts_tab] = pile[nb_elts_pile-1];
					pile[nb_elts_pile-1] = NULL;
					nb_elts_pile--;
					nb_elts_tab++;
				}
				
				/*On met un nouveau -o dans la pile*/
				if((pile[nb_elts_pile] = (commande*) calloc(1, sizeof(commande))) == NULL)
				{
					perror("calloc");
					exit(EXIT_FAILURE);
				}
				
				/*On lie à la fonction a*/
				pile[nb_elts_pile]->fonction = (void *)o;
				/*Les arguments sont mis a NULL. Ils seront mis en place lors de l'interprétation de la notation polonaise inversée.*/
				pile[nb_elts_pile]->arguments = NULL;
				pile[nb_elts_pile]->arg1 = NULL;
				pile[nb_elts_pile]->arg2 = NULL;
				pile[nb_elts_pile]->nb_args = 0;
				
				nb_elts_pile++;
				predicat = 0;
				operateur = 1;
			}
			else if(strcmp(argv[i], "-not") == 0)
			{
				if(predicat)
				{
					boolAnd = 1;
				}
				else
				{
					/*Il n'y a rien qui ai une priorité supérieur à -not donc on met directement dans la pile un nouveau -not.*/
					if((pile[nb_elts_pile] = (commande*) calloc(1, sizeof(commande))) == NULL)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					
					/*On lie à la fonction a*/
					pile[nb_elts_pile]->fonction = (void *)not;
					/*Les arguments sont mis a NULL. Ils seront mis en place lors de l'interprétation de la notation polonaise inversée.*/
					pile[nb_elts_pile]->arguments = NULL;
					pile[nb_elts_pile]->arg1 = NULL;
					pile[nb_elts_pile]->arg2 = NULL;
					pile[nb_elts_pile]->nb_args = 0;
					
					nb_elts_pile++;
					predicat = 0;
					operateur = 1;
					/*Si c'est la premiere chose lu après le chemin et les options lues on le dit*/
					if(!fonction_mal_place)
						fonction_mal_place = 1;
				}
			}
			
			/*Si nous entrons dans cette condtion c'est que l'utilisateur a entré une fonction ou un prédicat inconnu*/
			else
			{
				perror("findUs: prédicat inconnu");
				exit(EXIT_FAILURE);
			}
			
			/*S'il faut rajouté un -a entre deux prédicats*/
			if(boolAnd)
			{
				/*Si il y a eu un opérateur avant alors c'est qu'il y a erreur, cet opérateur est binaire*/
				if(operateur)
				{
					perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
					exit(EXIT_FAILURE);
				}
				
				/*Seul -not a une priorité supérieur à -a*/
				while(nb_elts_pile != 0 && pile[nb_elts_pile-1] != NULL && (pile[nb_elts_pile-1]->fonction == (void*)not || pile[nb_elts_pile-1]->fonction == (void*)a))
				{
					/*On met pile[nb_elts_pile] dans le resultat*/
					result[nb_elts_tab] = pile[nb_elts_pile-1];
					pile[nb_elts_pile-1] = NULL;
					nb_elts_pile--;
					nb_elts_tab++;
				}
				
				/*On met un nouveau -a dans la pile*/
				if((pile[nb_elts_pile] = (commande*) calloc(1, sizeof(commande))) == NULL)
				{
					perror("calloc");
					exit(EXIT_FAILURE);
				}
				
				/*On lie à la fonction a*/
				pile[nb_elts_pile]->fonction = (void *)a;
				/*Les arguments sont mis a NULL. Ils seront mis en place lors de l'interprétation de la notation polonaise inversée.*/
				pile[nb_elts_pile]->arguments = NULL;
				pile[nb_elts_pile]->arg1 = NULL;
				pile[nb_elts_pile]->arg2 = NULL;
				pile[nb_elts_pile]->nb_args = 0;
				
				nb_elts_pile++;
				predicat = 0;
				boolAnd = 0;
				operateur = 1;
				i--;
			}
		}
		
		/*A partir d'ici nous retrouvons des opérateurs (cf explications plus haut)*/
		else if(argv[i][0] == ',')
		{
			/*Si il y a eu un opérateur avant alors c'est qu'il y a erreur, cet opérateur est binaire*/
			if(operateur)
			{
				perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
				exit(EXIT_FAILURE);
			}
			
			/*Tous les autres prédicats ont une priorité supérieur à ,*/
			while(nb_elts_pile != 0 && pile[nb_elts_pile-1] != NULL && (pile[nb_elts_pile-1]->fonction == (void*)not || pile[nb_elts_pile-1]->fonction == (void*)a || pile[nb_elts_pile-1]->fonction == (void*)o || pile[nb_elts_pile-1]->fonction == (void*)virgule))
			{
				/*On met pile[nb_elts_pile] dans le resultat*/
				result[nb_elts_tab] = pile[nb_elts_pile-1];
				pile[nb_elts_pile-1] = NULL;
				nb_elts_pile--;
				nb_elts_tab++;
			}
			
			/*On met un nouveau , dans la pile*/
			if((pile[nb_elts_pile] = (commande*) calloc(1, sizeof(commande))) == NULL)
			{
				perror("calloc");
				exit(EXIT_FAILURE);
			}
			
			/*On lie à la fonction a*/
			pile[nb_elts_pile]->fonction = (void *)virgule;
			/*Les arguments sont mis a NULL. Ils seront mis en place lors de l'interprétation de la notation polonaise inversée.*/
			pile[nb_elts_pile]->arguments = NULL;
			pile[nb_elts_pile]->arg1 = NULL;
			pile[nb_elts_pile]->arg2 = NULL;
			pile[nb_elts_pile]->nb_args = 0;
			
			nb_elts_pile++;
			predicat = 0;
			operateur = 1;
		}
		/*Si l'argument est un '!'. Alors nous faisons comme un -not.*/
		else if(argv[i][0] == '!')
		{
			if(predicat)
			{
				boolAnd = 1;
			}
			else
			{
				/*Il n'y a rien qui ai une priorité supérieur à -not donc on met directement dans la pile un nouveau -not.*/
				if((pile[nb_elts_pile] = (commande*) calloc(1, sizeof(commande))) == NULL)
				{
					perror("calloc");
					exit(EXIT_FAILURE);
				}
				
				/*On lie à la fonction a*/
				pile[nb_elts_pile]->fonction = (void *)not;
				/*Les arguments sont mis a NULL. Ils seront mis en place lors de l'interprétation de la notation polonaise inversée.*/
				pile[nb_elts_pile]->arguments = NULL;
				pile[nb_elts_pile]->arg1 = NULL;
				pile[nb_elts_pile]->arg2 = NULL;
				pile[nb_elts_pile]->nb_args = 0;
				
				nb_elts_pile++;
				predicat = 0;
				operateur = 1;
				/*Si c'est la premiere chose lu après le chemin et les options lues on le dit*/
				if(!fonction_mal_place)
					fonction_mal_place = 1;
			}
		}
		
		/*Gestion des parenthèses.*/
		/*Si c'est une parenthèse gauche on la met sur la pile. La parenthèse gauche est représentée comme NULL dans la pile.*/
		/*Si c'est une parenthèse droite, alors on dépile les opérateurs et on les met dans la sortie jusqu’à la parenthèse gauche qui elle aussi sera dépilée,
		mais pas mise dans la sortie. Après cela, si le token au sommet de la pile est une fonction, le dépiler également pour l'ajouter à la sortie.
		Si toute la pile est dépilée sans trouver de parenthèse gauche c’est qu’il y a un mauvais parenthésage.*/
		else if(argv[i][0] == '(')
		{
			nb_parenthese_o++;
			nb_elts_pile++;
		}
		else if(argv[i][0] == ')')
		{
			nb_parenthese_f++;
			if(nb_parenthese_f > nb_parenthese_o)
			{
				perror("findUs: il y a trop de ')'");
				exit(EXIT_FAILURE);
			}
			
			/*On dépile tant qu'on a pas atteint la parenthèse ouvrante
			(ou qu'il reste ddes élèments dans la pile si il y a un problème de parenthésage
			(ce qui ne devrait jamais arrivé étant donné les 6 lignes précédentes)).*/
			while(nb_elts_pile > 0 && pile[nb_elts_pile-1] != NULL)
			{
				/*On met pile[nb_elts_pile] dans le resultat*/
				result[nb_elts_tab] = pile[nb_elts_pile-1];
				pile[nb_elts_pile-1] = NULL;
				nb_elts_pile--;
				nb_elts_tab++;
			}
			
			/*Si toute la pile est dépilée sans trouver de parenthèse gauche c’est qu’il y a un mauvais parenthésage.*/
			if(nb_elts_pile == 0)
			{
				perror("findUs: il y a trop de ')'");
				exit(EXIT_FAILURE);
			}
			
			/*On enlève la parenthèse ouvrante trouvée*/
			nb_elts_pile--;
			/*Si le token au sommet de la pile est un opérateur (c'est à dire s'il est non nul)
			* On le dépile et on l'ajoute à la sortie.*/
			if(nb_elts_pile > 0 && pile[nb_elts_pile-1] != NULL)
			{
				/*On met pile[nb_elts_pile] dans le resultat*/
				result[nb_elts_tab] = pile[nb_elts_pile-1];
				pile[nb_elts_pile-1] = NULL;
				nb_elts_pile--;
				nb_elts_tab++;
			}
		}
		
		/*Sinon il y a plusieurs cas possibles :
		-Si un prédicat ou une fonction a déjà été lu ça veux dire que l'utilisateur a fait donné une commande inconnue.
		-Si l'utilisateur a précisé un dossier alors nous le récupérons.
		-S'il a précisé un fichier alors idem.*/
		else
		{
			/*Si on a déjà lu un opérateur, un prédicat ou une fonction alors c'est qu'il y a un problème.*/
			if(f_presente || fonction_mal_place || op_present)
			{
				perror("findUs: les chemins doivent précéder les prédicats ou options.");
				exit(EXIT_FAILURE);
			}
			
			if(!nb_elts_tab && !p->path)
			{
				/*Premier argument = dossier*/
				if(argv[i][strlen(argv[i])-1] == '/')
				{
					p->path = calloc(strlen(argv[i]) + 1, sizeof(char));
					if(!p->path)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					strcpy(p->path, argv[i]);
				}
				else
				{
					p->path = calloc(strlen(argv[i]) + 2, sizeof(char));
					if(!p->path)
					{
						perror("calloc");
						exit(EXIT_FAILURE);
					}
					sprintf(p->path, "%s/", argv[i]);
				}
			}
		}
	}
	
	/*Si il y a trop de parenthèses*/
	if(nb_parenthese_o != nb_parenthese_f)
	{
		perror("findUs: expression non valide ; ')' attendue mais non détectée.");
		exit(EXIT_FAILURE);
	}
	/*S'il reste des éléments dans la pile des opérateurs. Alors on les dépilent tous.
	Normalement il ne doit pas y avoir de parenthèses (cela est vérifié juste au dessus).
	Mais si jamais il y en a une nous quittons.*/
	if(nb_elts_pile != 0)
	{
		/*Tant que nb_elts_tab n'est pas nul on met pile[nb_elts_pile] dans le resultat*/
		for( ; nb_elts_pile > 0; nb_elts_pile--)
		{
			/*Si c'est une parenthèse*/
			if(pile[nb_elts_pile-1] == NULL)
			{
				perror("findUs: expression non valide ; ')' attendue mais non détectée.");
				exit(EXIT_FAILURE);
			}
			
			result[nb_elts_tab] = pile[nb_elts_pile-1];
			pile[nb_elts_pile-1] = NULL;
			nb_elts_tab++;
		}
	}
	
	/*Reste à appeler l'interpreteur de la notation polonaise inversée.*/
	analyse_pol_inv(p, nb_elts_tab);
	
	/*On libère la mémoire utilisée*/
	free(pile);
	
	return p;
}

/*Fonction d'interprétation du tableau de commande livré en paramètre (param->cmds) qui doit être structuré en notation polonaise inversée.*/
void analyse_pol_inv(parametres* param, int nb_elts_tab)
{
	int i;
	/*Le tableau result contiendra le résultat de la conversion des paramètres en arbre executable.*/
	commande ** polonais = NULL;
	commande ** result = NULL;
	int nb_elts_result = 0;
	/*On branche notre résultat à la place de cmds et on met cmds dans polonais*/
	polonais = param->cmds;
	/*Il y aura au plus argc commande donc nous créons un tableau de taille nb_elts_tab+1*/
	if((result = (commande**) calloc(nb_elts_tab+1, sizeof(commande*))) == NULL)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	param->cmds = result;
	
	/*On transforme notre polonais inversé en arbre. Le tableau de résultat va nous servir de pile.
	Pour cela on lit le tableau polonais. Si ce qu'on lit n'est pas un opérateur alors on le met dans le résultat.
	Sinon l'opérateur lu dépile le bon nombre d'arguments dans la pile des résultats (donc 1 ou 2) puis s'empile dans resultat.*/
	for(i = 0; i < nb_elts_tab; i++)
	{
		/*Si c'est l'un des opérateur binaire*/
		if(polonais[i]->fonction == (void*)a || polonais[i]->fonction == (void*)o || polonais[i]->fonction == (void*)virgule)
		{
			/*Premier argument*/
			if(nb_elts_result <= 0)
			{
				perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
				exit(EXIT_FAILURE);
			}
			polonais[i]->arg1 = result[nb_elts_result-1];
			result[nb_elts_result-1] = NULL;
			nb_elts_result--;
			
			/*Second argument*/
			if(nb_elts_result <= 0)
			{
				perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
				exit(EXIT_FAILURE);
			}
			polonais[i]->arg2 = result[nb_elts_result-1];
			result[nb_elts_result-1] = NULL;
			nb_elts_result--;
			
			/*On met l'opérateur dans la pile des résultats*/
			result[nb_elts_result] = polonais[i];
			polonais[i] = NULL;
			nb_elts_result++;
		}
		
		/*Si c'est l'opérateur unaire*/
		else if(polonais[i]->fonction == (void*)not)
		{
			/*Second argument*/
			if(nb_elts_result <= 0)
			{
				perror("findUs: expression non valide ; vous avez utilisé un opérateur binaire non précédé d'une expression.");
				exit(EXIT_FAILURE);
			}
			polonais[i]->arg2 = result[nb_elts_result-1];
			result[nb_elts_result-1] = NULL;
			nb_elts_result--;
			
			/*On met l'opérateur dans la pile des résultats*/
			result[nb_elts_result] = polonais[i];
			polonais[i] = NULL;
			nb_elts_result++;
		}
		
		/*Sinon ce n'est pas l'un des opérateur alors on empile le prédicat*/
		else
		{
			result[nb_elts_result] = polonais[i];
			polonais[i] = NULL;
			nb_elts_result++;
		}
	}
	
	/*On libère le tableau du polonais inversé.*/
	free(polonais);
}

/*Prend une série de commandes ainsi qu'une entrée de répertoire, et vérifie si l'entrée correspond à ce qu'on recherche.*/
int execute_commande(commande ** cmds, struct dirent * entry, struct stat * st, char * chemin)
{
	int i = 0;
	int retour = 1;
	
	/*cmds finit par un pointeur NULL*/
	for(i=0;cmds[i];i++)
	{
		if(cmds[i]->fonction)
			retour = retour && cmds[i]->fonction(cmds[i], entry, st, chemin);
		if(!retour) break;
	}
	
	return retour;
}

int true(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return 1;
}

int false(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return 0;
}

int name(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	int val_return;
	char * name = calloc(strlen(d->d_name)+1, sizeof(char));
	strcpy(name, d->d_name);
	if(!strcmp(".", name))
	{
		if(strcmp(chemin, "/"))
		{
			int i, j;
			free(name);
			for(i=strlen(chemin)-2; i>=0 && chemin[i] != '/'; i--);
			name = calloc(strlen(chemin) + 1 - i, sizeof(char));
			for(i++, j=0; i<strlen(chemin)-1; name[j] = chemin[i], i++, j++);
		}
		else
		{
			name[0] = '/';
		}
	}
	val_return = !fnmatch(c->arguments[0], name, 0);
	free(name);
	
	return val_return;
}

int type(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return d->d_type == charTypeToInt(c->arguments[0][0]);
}

int uid(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return st->st_uid == atoi(c->arguments[0]);
}

int gid(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return st->st_gid == atoi(c->arguments[0]);
}

int user(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	struct passwd * pw = getpwuid(st->st_uid);
	return !strcmp(pw->pw_name, c->arguments[0]);
}

int group(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	struct group * gr = getgrgid(st->st_gid);
	return !strcmp(gr->gr_name, c->arguments[0]);
}

int atime(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	time_t t;
	time(&t);
	int n = atoi(c->arguments[0]);
	
	return (n<0) ? (int)difftime(t, st->st_atime)/(60*60*24) <= (-n+1) : (int)difftime(t, st->st_atime)/(60*60*24) >= (n+1);
}

int ctime_(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	time_t t;
	time(&t);
	int n = atoi(c->arguments[0]);
	
	return (n<0) ? (int)difftime(t, st->st_ctime)/(60*60*24) <= (-n+1) : (int)difftime(t, st->st_ctime)/(60*60*24) >= (n+1);
}

int mtime(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	time_t t;
	time(&t);
	int n = atoi(c->arguments[0]);
	
	return (n<0) ? (int)difftime(t, st->st_mtime)/(60*60*24) <= (-n+1) : (int)difftime(t, st->st_mtime)/(60*60*24) >= (n+1);
}

int perm(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	int i, retour;
	char * perm = c->arguments[0];
	char * perm_fichier;
	int mode = 0;
	/*
	 * 0 = normal
	 * 1 = masque /
	 * 2 = au moins -
	 */
	 
	if(!(*perm >= '0' && *perm <= '9'))
	{
		if(*perm == '/') mode = 1;
		else mode = 2;
		
		perm++;
	}
	
	perm_fichier = calloc(5, sizeof(char));
	sprintf(perm_fichier, "%04o", st->st_mode & 0777);
	
	if(strlen(perm) == 3) perm_fichier++;
	if(strlen(perm) == 2) perm_fichier += 2;
	if(strlen(perm) == 1) perm_fichier += 3;
	
	if(!mode)
	{
		//Normal
		retour = 1;
		for(i=0;i<strlen(perm);i++)
		{
			retour = retour && (perm_fichier[i] == perm[i]);
		}
	}
	else if(mode == 1)
	{
		//Masque
		retour = 0;
		for(i=0;i<strlen(perm);i++)
		{
			retour = retour || (perm_fichier[i] == perm[i]);
		}
	}
	else
	{
		//Au moins
		retour = 1;
		for(i=0;i<strlen(perm);i++)
		{
			retour = retour && (perm_fichier[i] >= perm[i]);
		}
	}
	
	if(strlen(perm) == 3) perm_fichier--;
	if(strlen(perm) == 2) perm_fichier -= 2;
	if(strlen(perm) == 1) perm_fichier -= 3;
	free(perm_fichier);
	
	return retour;
}

int print(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	printf("%s\n", chemin);
	return 1;
}

int ls(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	struct passwd * pw;
	struct group * gr;
	struct tm * timeStruct;
	char buf[1000];
	int size;
	char * mode;
	
	pw = getpwuid(st->st_uid);
	gr = getgrgid(st->st_gid);
	timeStruct = localtime(&st->st_mtime);
	mode = mode2string(st->st_mode);
	
	if(!S_ISLNK(st->st_mode))
	{
		if(pw && gr)
		{
			printf("%06d\t%04d\t%s\t%03d\t%8s\t%8s\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, pw->pw_name, gr->gr_name, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin);
		}
		else if(!pw && gr)
		{
			printf("%06d\t%04d\t%s\t%03d\t%08d\t%8s\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, (int)st->st_uid, gr->gr_name, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin);
		}
		else if(!gr && pw)
		{
			printf("%06d\t%04d\t%s\t%03d\t%8s\t%08d\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, pw->pw_name, (int)st->st_gid, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin);
		}
		else
		{
			printf("%06d\t%04d\t%s\t%03d\t%08d\t%08d\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, (int)st->st_uid, (int)st->st_gid, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin);
		}
	}
	else
	{
		if(-1 != (size = readlink(chemin, buf, 1000)))
		{
			buf[size] = 0;
			if(pw && gr)
			{
				printf("%06d\t%04d\t%s\t%03d\t%8s\t%8s\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s -> %s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, pw->pw_name, gr->gr_name, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin, buf);
			}
			else if(!pw && gr)
			{
				printf("%06d\t%04d\t%s\t%03d\t%08d\t%8s\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s -> %s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, (int)st->st_uid, gr->gr_name, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin, buf);
			}
			else if(!gr && pw)
			{
				printf("%06d\t%04d\t%s\t%03d\t%8s\t%08d\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s -> %s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, pw->pw_name, (int)st->st_gid, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin, buf);
			}
			else
			{
				printf("%06d\t%04d\t%s\t%03d\t%08d\t%08d\t%08d\t%04d-%02d-%02d\t%02d:%02d\t%s -> %s\n", (int)st->st_ino, (int)st->st_blocks/2, mode, (int)st->st_nlink, (int)st->st_uid, (int)st->st_gid, (int)st->st_size, 1900 + timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday, timeStruct->tm_hour, timeStruct->tm_min, chemin, buf);
			}
		}
	}
	
	free(mode);
	
	return 1;
}

int exec(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	//execvp
	pid_t pid, w;
	int status, i;
	char ** arguments = null;
	
	switch((pid = fork()))
	{
		case -1 :
			perror("fork");
			exit(EXIT_FAILURE);
		break;
		case 0 :
			arguments = calloc(c->nb_args, sizeof(char *));
			for(i=0; i<c->nb_args; i++)
			{
				if(!strcmp(c->arguments[i], "{}"))
				{
					arguments[i] = calloc(strlen(chemin) + 1, sizeof(char));
					strcpy(arguments[i], chemin);
				}
				else
				{
					arguments[i] = calloc(strlen(c->arguments[i]) + 1, sizeof(char));
					strcpy(arguments[i], c->arguments[i]);
				}
			}
			execvp(arguments[0], arguments);
			perror("execvp");
			exit(EXIT_FAILURE);
		break;
		default:
			do
			{
		 		w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
				if (w == -1)
				{
					perror("waitpid");
					exit(EXIT_FAILURE);
				}
		
				if (WIFEXITED(status))
				{
					return !WEXITSTATUS(status);
				}
				else if (WIFSIGNALED(status))
				{
					return 0;
				}
				else if (WIFSTOPPED(status))
				{
					return 0;
				}
				else if (WIFCONTINUED(status))
				{
					return 0;
				}

		
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		break;
	}
	
	return 0;
}

int prune(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return 1;
}

int a(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return c->arg1->fonction(c->arg1, d, st, chemin) && c->arg2->fonction(c->arg2, d, st, chemin);
}

int virgule(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	c->arg1->fonction(c->arg1, d, st, chemin);
	return (int) c->arg2->fonction(c->arg2, d, st, chemin);
}

int o(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return c->arg1->fonction(c->arg1, d, st, chemin) || c->arg2->fonction(c->arg2, d, st, chemin);
}

int not(struct commande * c, struct dirent * d, struct stat * st, char * chemin)
{
	return !c->arg2->fonction(c->arg2, d, st, chemin);
}

int charTypeToInt(char type)
{
	switch(type)
	{
		case 'f':
			return DT_REG;
		break;
		case 'd':
			return DT_DIR;
		break;
		case 'p':
			return DT_FIFO;
		break;
		case 's':
			return DT_SOCK;
		break;
		case 'c':
			return DT_CHR;
		break;
		case 'b':
			return DT_BLK;
		break;
		case 'l':
			return DT_LNK;
		break;
		default:
			return DT_UNKNOWN;
		break;
	}
}
