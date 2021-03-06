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
#include "travel.h"

int main(){
	//Initialization
	srand(time(NULL));
	strcpy(dir[0],"North");
	strcpy(dir[1],"East");
	strcpy(dir[2],"South");
	strcpy(dir[3],"West");
	strcpy(dir[4],"Up");
	strcpy(dir[5],"Down");

	//Welcome Message
	printf("Welcome to the Veins of the Earth semi-autonomous GM.\n\n");

	cavesystem *csys;
	int stillplaying = 1;
	while(stillplaying){
		printf("\n\n");
		printf("Please select an option.\n");
		printf("A: Generate a cave system\n");
		printf("B: Read a cave system\n");
		printf("Q: Exit\n");
		fflush(stdout);

		//Get input
		int input = getchar();
		fflush(stdin);
		input = toupper(input);

		//Do Stuff
		switch(input){
			case 'A' :
				csys = generateCaveSystem();
				fflush(stdout);
				printCaveSystem(csys);
				break;
			case 'B' :
				;//Just go with it
				int startcave = 0;
				travelCave(csys->caves[startcave]);
				break;
			case 'Q' :
				destroyCaveSystem(csys);
				csys = NULL;
				stillplaying = 0;
				break;
			default:
				printf("Try again.\n");
				break;
		}
	}
}