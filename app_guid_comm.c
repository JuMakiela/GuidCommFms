#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ivy.h>
#include <ivyloop.h>

float roll_cmd;
char cmd[100] = "GC_CMD_ROLL = INIT";
bool active = True;

/* fonction associe a  */
void calculRoulis(IvyClientPtr app, void *data, int argc, char **argv){
	int time;
	float XTK, TAE, DTWPT;
	int roll_commande;
	char retour[100] = "GC_CMD_ROLL =";
	const char* arg = (argc < 1) ? "" : argv[0];
	fprintf(stderr,"%s\n",arg);
	//GS_DATA Time="time" XTK="" TAE="" Dist_to_WPT=""
	sscanf(arg,"GS_DATA Time=%d XTK=%d TAE=%d Dist_to_WPT=%d",time, XTK, TAE, DTWPT); // GS_XTK_TAE = time
	//fonction de check time/donnee
	roll_commande = 200; //Calcul de la commande de roulis
	if(active){ //On transmet la commande si le PA est actif
	    strcat(retour, actual_time);
	    strcat(retour,roll_commande);
	    IvySendMsg ("%s", retour);
	    //roll_cmd = retour;
	    //calcul = 0;
	}
}
/* fonction associe a l'horloge */

void envoi(IvyClientPtr app, void *data, int argc, char **argv){

}
/*
on verifie si le calculRoulis a eu lieu
si oui
on check si on doit envoyer notre info (100ms)
si non 
on envoi la commande précédente
si 1s 
on desarme le PA
void Sender(IvyClientPtr app, void *data, int argc, char **argv){
    char retour[100] = "GC_CMD_ROLL =";
    if(calcul < 100){
        if(clocl%100=90){ //envoi tout les 100ms à 90ms
	        IvySendMsg ("%s", roll_cmd);
	        calcul++;
        }
    }
    else{active = False;}
}

IMPORTANT définir avec le groupe seq la periode de l'horloge 10 ou 20 ms
*/

/* fonction associe a  */
void stop (IvyClientPtr app, void *data, int argc, char **argv){
	IvyStop ();
}

int main (int argc, char**argv){

	/* handling of -b option */
	const char* bus = 0;
	if( argc == 3){
		
		if(strcmp(argv[1], "-b") == 0){
			bus = argv[2];
		}
		else{
			printf("Definir un bus -b 127.127.127.127:2010\n");
			exit(1);
		}
	}
	else{
		printf("Definir un bus -b 127.127.127.127:2010\n");
		exit(1);
	}

	/* initialisation */
	IvyInit ("Guid_COMM_APP", "Bonjour de Guid COMM", 0, 0, 0, 0);
	IvyStart (bus);
	/* abonnement  */
	IvyBindMsg (calculRoulis, 0, "GS_Data Time=(.*) XTK=(.*) TAE=(.*) Dist_to_WPT=(.*)"); //GS_Data Time="time" XTK=" " TAE=" " Dist_to_WPT=" "
	/* abonnement */
	IvyBindMsg (stop, 0, "^Stop$");
	/* abonnement */
	//on s'abonne à l'holorge qui cadence nos envois
	IvyBindMsg (envoi, 0, "^Time=(.*)");
	/* main loop */
	IvyMainLoop();
	return 0;
}
