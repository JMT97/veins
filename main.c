#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "veinsutil.h"
#include "cavestructures.h"
#include "generation.h"
#include "printing.h"

int main(){
	//Initialization
	srand(time(NULL));
	strcpy(dir[0],"North");
	strcpy(dir[1],"East");
	strcpy(dir[2],"South");
	strcpy(dir[3],"West");
	strcpy(dir[4],"Up");
	strcpy(dir[5],"Down");

	//Main menu
	printf("Welcome to the Veins of the Earth semi-autonomous GM.\n");
	printf("Please select an option.\n");
	printf("A: Generate a cave system\n");
	printf("B: Read a cave system\n");
	fflush(stdout);

	//Get input
	int input = getchar();
	input = toupper(input);

	//Do Stuff
	switch(input){
		case 'A' :
			;//just go with it
			cave *gen = generateCave();
			printCave(gen);

			cave *gen2 = generateCave();
			printCave(gen2);

			route *gen3 = generateRoute(gen,gen2,0,0);
			printRoute(gen3);
			
			free(gen);
			free(gen2);
			free(gen3);
			break;
		case 'B' :
			;//Just go with it
			cavesystem *cs = generateCaveSystem();
			fflush(stdout);
			printCaveSystem(cs);
			break;
	}

}