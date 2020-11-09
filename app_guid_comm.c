#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ivy.h>
#include <ivyloop.h>

/* fonction associe a  */
void HelloCallback (IvyClientPtr app, void *data, int argc, char **argv){
	const char* arg = (argc < 1) ? "" : argv[0];
	fprintf(stderr,"%s\n",arg);
	//scanf(
	IvySendMsg ("Bonjour%s", arg);
}

/* fonction associe a  */
void ByeCallback (IvyClientPtr app, void *data, int argc, char **argv){
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
	IvyBindMsg (HelloCallback, 0, "^Hello(.*)");
	/* abonnement */
	IvyBindMsg (ByeCallback, 0, "^Bye$");
	/* main loop */
	IvyMainLoop();
	return 0;
}
