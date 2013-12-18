
#ifndef FINDUS
#define FINDUS

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fnmatch.h>
#include <time.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>

#define null NULL

typedef struct retour
{
	char*** retour;
	int* taille;
} Retour;

typedef struct commande
{
	/*Fonction de la commande*/
	void* (*fonction)(struct commande *, struct dirent *, struct stat *, char *);
	
	/*Arguments : Si la fonction est un 'et' ou un 'ou' alors nous utiliserons arg1 et arg2 dans tous les autres cas nous utiliserons argments*/
	char** arguments;
	struct commande* arg1;
	struct commande* arg2;
	int nb_args;
} commande;

typedef struct parametres
{
	commande ** cmds;
	int mindepth; /*-1 si pas spécifiée*/
	int maxdepth; /*-1 si pas spécifiée*/
	int sort; /*booléen*/
	int prune; /*booléen*/
	int ls; /*booléen*/
	int action;
	char * path;
} parametres;

/*Find*/
char * getCheminDepart(parametres * params);
void findFiles(parametres * params, char * chemin, int niveau, char *** retour, int * taille);
Retour* find(int argc, char ** argv);
char * mode2string(mode_t m);
void detruire_commande(commande * cmd);

int true(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int false(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int name(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int type(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int uid(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int gid(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int user(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int group(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int atime(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int ctime_(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int mtime(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int perm(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int print(struct commande * c, struct dirent * d, struct stat * st, char * chemin);
int ls(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int exec(struct commande * c, struct dirent * d, struct stat * st, char * chemin);
int prune(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int a(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int o(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int not(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //
int virgule(struct commande * c, struct dirent * d, struct stat * st, char * chemin); //

/*Lecture des paramètres*/
/*lecture_param lit et interprête les parametres en deuxtemps :
- elle convertie les chaînes de caractères passée en paramètres en notation polonaise inversée.
- puis elle appel la fonction analyse_pol_inv qui traduit la notation polonaise inversée en sa forme définitive.*/
parametres * lecture_param(int argc, char * argv[]);
void analyse_pol_inv(parametres* param, int nb_elts_tab);
int execute_commande(commande ** cmds, struct dirent * entry, struct stat * st, char * chemin);
int charTypeToInt(char type);

#endif
