#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ivy.h>
#include <ivyloop.h>

float roll_cmd;
char retour[100] = "GC_CMD_ROLL = INIT";
int active = 1;
int calcul = 0;

/* fonction associe a  */
void CalculRoulis(IvyClientPtr app, void *data, int argc, char **argv){
	int time;
	float XTK, TAE, DTWPT, roll_commande;
	char retour[100] = "GC_CMD_ROLL =";
	char tm[20], r_cmd[20];
	const char* arg = (argc < 1) ? "" : argv[0];
	fprintf(stderr,"%s\n",arg);
	//GS_DATA Time="time" XTK="" TAE="" Dist_to_WPT=""
	sscanf(arg,"Time=%d XTK=%f TAE=%f Dist_to_WPT=%f",&time, &XTK, &TAE, &DTWPT); // GS_XTK_TAE = time
	//fonction de check time/donnee
	roll_commande = 200; //Calcul de la commande de roulis
	sprintf(tm, "%d", time);
	sprintf(r_cmd, "%f", roll_commande);
	if(active){ //On transmet la commande si le PA est actif
	    strcat(retour, tm); //actual_time
	    strcat(retour, r_cmd);
	    //IvySendMsg ("%s", retour);
	    calcul = 0;
	}
}
/* fonction associe a l'horloge */
/*
on verifie si le calculRoulis a eu lieu
si oui
on check si on doit envoyer notre info (100ms)
si non 
on envoi la commande précédente
si 1s 
on desarme le PA*/
void Sender(IvyClientPtr app, void *data, int argc, char **argv){
    int cl;
    const char* arg = (argc < 1) ? "" : argv[0];
    sscanf(arg,"%d",&cl);
    char retour[100] = "GC_CMD_ROLL =";
    if(calcul < 100){
        if((cl%100)==90){ //envoi tout les 100ms à 90ms
	        IvySendMsg ("%s", retour);
	        calcul++;
        }
    }
    else{
        active = 0;
    }
}
/*
IMPORTANT définir avec le groupe seq la periode de l'horloge 10 ou 20 ms
*/

/* fonction associe a  */
void Stop (IvyClientPtr app, void *data, int argc, char **argv){
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
	IvyBindMsg (CalculRoulis, 0, "GS_DATA(.*)");
	/* abonnement */
	IvyBindMsg (Stop, 0, "^Stop$");
	/* abonnement */
	//on s'abonne à l'holorge qui cadence nos envois
	IvyBindMsg (Sender, 0, "^Time=");
	/* main loop */
	IvyMainLoop();
	return 0;
}
