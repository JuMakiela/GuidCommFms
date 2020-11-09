#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ivy.h>
#include <ivyloop.h>
/* callback associated to "Hello" messages */
void HelloCallback (IvyClientPtr app, void *data, int argc, char **argv){
	const char* arg = (argc < 1) ? "" : argv[0];
	fprintf(stderr,"%s\n",arg);
	IvySendMsg ("Bonjour%s", arg);
}
/* callback associated to "Bye" messages */
void ByeCallback (IvyClientPtr app, void *data, int argc, char **argv){
	IvyStop ();
}
int main (int argc, char**argv){

	/* initializations */
	IvyInit ("IvyTranslater", "Hello le monde", 0, 0, 0, 0);
	IvyStart ("10.3.141.120:2010");
	/* binding of HelloCallback to messages starting with 'Hello' */
	IvyBindMsg (HelloCallback, 0, "^Hello(.*)");
	/* binding of ByeCallback to 'Bye' */
	IvyBindMsg (ByeCallback, 0, "^Bye$");
	/* main loop */
	IvyMainLoop();
	return 0;
}
