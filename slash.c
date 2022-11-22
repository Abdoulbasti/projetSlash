#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char dernier_sym[PATH_MAX] = "\0";

//insère dans "res" un string contenant les caractères jusqu'au premier "/" de "chemin"
void prochain_dossier(char * res, char * chemin){
	res[0] = '\0';
	int len = strlen(chemin);
	int i;
	for(i = 0; i<len && chemin[i]!='/'; i++){
		res[i] = chemin[i];
	}
	res[i] = '\0';
}

//enlève les caractères jusqu'au premier "/"
void enlever_premier_dossier(char * chemin){
	char tmp[PATH_MAX];
	tmp[0] = '\0';
	int len = strlen(chemin);
	int i;
	for(i = 0; i<len && chemin[i]!='/'; i++){}
	sprintf(tmp, "%s", chemin);
	if(i == len){
		chemin[0] = '\0';
	}
	else{
		sprintf(chemin, "%s", tmp+i+1);
	}
}

//enlève les charactères à partir du dernier "/"
void enlever_dernier_dossier(char * chemin){
	int len = strlen(chemin);
	int i;
	for(i = len; i>0 && chemin[i]!='/'; i--){}
	chemin[i]='\0';
}

void cd(int argc, char **argv){
	/*printf("argc: %i\n", argc);
	for(int i = 0; i<argc; i++){
		printf("argv[%i]: %s\n", i, argv[i]);
	}*/
	if(strcmp(dernier_sym, "\0") == 0){
		sprintf(dernier_sym, "%s", getenv("PWD"));
	}
	if(argc == 0){
		DIR * dir = opendir(getenv("HOME"));
		if(dir == NULL){
			perror("erreur ouverture dossier ~");
			exit(1);
		}
		else{
			closedir(dir);
		}
		setenv("PWD", getenv("HOME"), 1);
	}
	else if(argc == 1 && strcmp(argv[0],"-")==0){
		//ouvre le dernier répertoire puis échange les deux chemins symboliques
		DIR * courant = opendir(dernier_sym);
		if(courant == NULL){
			perror("erreur ouverture dossier");
			exit(1);
		}
		else{
			closedir(courant);
		}
		char tmp[PATH_MAX];
		sprintf(tmp, "%s", dernier_sym);
		sprintf(dernier_sym, "%s", getenv("PWD"));
		setenv("PWD", tmp, 1);
	}
	else if(argc == 1 || (argc == 2 && strcmp(argv[0],"-L")==0)){
		char param[PATH_MAX];
		if(argc == 1){
			sprintf(param, "%s", argv[0]);
		}
		else{
			sprintf(param, "%s", argv[1]);
		}
		//chemin interprété de manière logique
		//si la référence logique n'a pas de sens, l'interpréter de manière physique
		if(param[0]=='/'){
			DIR * d = opendir(param);
			if(d == NULL){
				perror("le chemin n'existe pas");
				exit(1);
			}
			else{
				closedir(d);
			}
			sprintf(dernier_sym, "%s", getenv("PWD"));
			setenv("PWD", param, 1);
		}
		else{
			//stocke le nouveau chemin symbolique. Il sera écrit dans la variable "pwd" si le chemin est valide
			char tmp[PATH_MAX];
			//premier dossier dans le chemin à parcourir
			char * nom_dossier[PATH_MAX];
			//copie de la valeur de "$PWD" dans "tmp"
			sprintf(tmp, "%s", getenv("PWD"));
			while(param[0]!='\0'){
				char nom_dossier[PATH_MAX];
				//insertion du nom du prochain dossier à parcourir dans nom_dossier
				printf("\n");
				prochain_dossier(nom_dossier, param);
				printf("tmp: %s\n", tmp);
				printf("param: %s\n", param);
				printf("nom_dossier: %s\n", nom_dossier);
				if(strcmp(nom_dossier, "..")==0){
					if(strcmp(tmp, "/")==0){
						perror("chemin incorrect");
						exit(1);
					}
					else{
						enlever_premier_dossier(param);
						enlever_dernier_dossier(tmp);
					}
				}
				else if(strcmp(nom_dossier, ".")==0){
					enlever_premier_dossier(param);
				}
				else{
					//ajout du répertoire courant à la fin de tmp
					strcat(tmp, "/");
					strcat(tmp, nom_dossier);

					DIR * d = opendir(tmp);
					if(d == NULL){
						perror("répertoire inexistant");
						exit(1);
					}
					else{
						closedir(d);
					}
					enlever_premier_dossier(param);
				}
			}
			sprintf(dernier_sym, "%s", getenv("PWD"));
			setenv("PWD", tmp, 1);
		}

	}
	else if(argc == 2 && strcmp(argv[0],"-P")==0){
		//naviguer à travers les répertoires sans avoir besoin de gérer de string. Appeler pwd à la fin 
		//comment ne pas ouvrir les inoeuds? 
		if(argv[1][0]=='/'){
			//ouvrir le dossier "/"
			//faire une boucle comme dans l'interprétation symbolique en vérifiant les répertoires à chaque fois
			DIR * d = opendir(argv[2]);
			if(d == NULL){
				perror("le chemin n'existe pas");
			}
			else{
				closedir(d);
			}
			sprintf(dernier_sym, "%s", getenv("PWD"));
			setenv("PWD", argv[1], 1);
		}
	}
	else{
		//afficher les instruction d'utilisation
		//exit(1);
	}

}

int main(int argc, char **argv){
	//printf("PWD: %s\n", getenv("PWD"));
	//cd();
	/*char chemin[PATH_MAX] = "fiubbukvd/dfdtstbdtebtb/drdtrbtstbbtd/sdtdtstdstb";
	char nom_dossier[PATH_MAX];
	printf("chemin: %s\n", chemin);
	prochain_dossier(nom_dossier, chemin);
	printf("prochain_dossier: %s\n", nom_dossier);
	enlever_premier_dossier(chemin);
	printf("enlever_premier_dossier: %s\n", chemin);
	enlever_dernier_dossier(chemin);
	printf("enlever_dernier_dossier: %s\n", chemin);*/
	printf("Dossier courant: %s\n", getenv("PWD"));
	cd(argc-1, argv+1);
	printf("Dossier courant: %s\n", getenv("PWD"));
	/*char b[PATH_MAX] = "jkbklklbjlkjblk";
	enlever_premier_dossier(b);
	printf("%s\n", b);*/
}