#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include <ivy.h>
#include <ivyloop.h>
#include <Ivy/timer.h>

#define PERIODE 100

/*Variable globale associé a l'horloge*/
int i = 0;

/* fonction associe a l'emission de l'horloge */
void HorlogeCallback(TimerId id, void *user_data, unsigned long delta) {
	IvySendMsg ("Time = %d",i*PERIODE);
	i++;
}

int main(int argc, char**argv){

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
	
	IvyInit("Horloge", "Bonjour de l'horloge", 0, 0, 0, 0);
	IvyStart(bus);
	TimerRepeatAfter(TIMER_LOOP, PERIODE, HorlogeCallback, 0); 
	IvyMainLoop();
	return 0;
}
