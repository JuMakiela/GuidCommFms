#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ivy.h>
#include <ivyloop.h>

/* fonction associe a  */
void CalculRoulis(IvyClientPtr app, void *data, int argc, char **argv){
	int time, XTK, TAE;
	int roll_commande;
	char retour[100] = "GC_CMD_ROLL =";
	const char* arg = (argc < 1) ? "" : argv[0];
	fprintf(stderr,"%s\n",arg);
	sscanf(arg,"%d %d %d",time, XTK, TAE); // GS_XTK_TAE = time
	//fonction de check time/donnee
	roll_commande = 200; //Calcul de la commande de roulis
	
	strcat(retour, actual_time);
	strcat(retour,roll_commande);
	IvySendMsg ("%s", retour);
}
/* fonction associe a l'horloge */
/*
on verifie si le calculRoulis a eu lieu
si oui
on check si on doit envoyer notre info (100ms)
si non 
on envoi la commande précédente
si 1s 
on desarme le PA


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
	IvyBindMsg (CalculRoulis, 0, "GS_XTK_TAE = (.*)");
	/* abonnement */
	IvyBindMsg (Stop, 0, "^Stop$");
	/* abonnement */
	//on s'abonne à l'holorge qui cadence nos envois
	/* main loop */
	IvyMainLoop();
	return 0;
}
