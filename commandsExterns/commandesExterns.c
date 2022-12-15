#include "commandesExterns.h"



void gestionErreur(char* nomFonction)
{
    perror(nomFonction);
    exit(1);
}


/*
Execution de tous les autres commandes se trouvant à un autre emplacement que celui du path.
*/
int executionCommandesExternesAutres(char* arguments[MAX_ARGS_NUMBER])
{
    int codeRetour = 1;
    int retourExec  = execv(arguments[0], arguments);
    char* exec = "execv";
    if(retourExec == -1) 
    {
        codeRetour = 1;
        printf("exection de %s a echoue \n", exec);
        gestionErreur(exec);
    }

    return codeRetour;
}


/*
Execution des commandes extern
    -> On execute d'abord la commande dans path
    -> Si ça echoue on execute alors la commandes se trouvant à un emplacement fournis par l'utilisateur
*/
int executionCommandeExternes(char* arguments[MAX_ARGS_NUMBER])
{
    int codeRetour =1;
    int retourExec = execvp(arguments[0], arguments);
    char* exec = "execvp";
    
    //Si l'execution d'une commande de Path echou on essaye alors celui se trouvant dans un autre emplacement
    if (retourExec == -1)
    {
        printf("exection de %s a echoue \n", exec);
        codeRetour = executionCommandesExternesAutres(arguments);
    }

    return codeRetour;
}


/*
Execute l'ensemble des commandes externes installer dans la machine et 
tout les autres commandes se trouvant à un emplacement fournis par 
l'utilisateur.
*/
int commandesExternes(char* commandes[MAX_ARGS_NUMBER])
{
    char* nomFonction = "fork";
    
    pid_t pid = fork();
    int codeRetour;
    if(pid == -1) 
    { 
        gestionErreur(nomFonction); 
        codeRetour = 1;
    }
    else if(pid == 0)
    {
        codeRetour = executionCommandeExternes(commandes);
    }
    else 
    {  
        int statusWait;
        int retourWait  = wait(&statusWait);
        nomFonction = "wait";
        if (retourWait == -1) 
        { 
            gestionErreur(nomFonction);
            codeRetour = 1;
        }
    }

    return codeRetour;
}