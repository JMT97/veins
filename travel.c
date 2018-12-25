#include "travel.h"

#include "cavestructures.h"
#include "veinsutil.h"
#include "printing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void travelCave(cave* c){
	printCave(c);
	printf("\n");
	for(int i = 0; i < MAX_DIR; i++){
		if(c->entrancesizes[i]>0){
			printf("%c: Exit from the %s.\n",'A'+i,dir[i]);
		}
	}
	printf("Q: Exit\n");

	int input = 1;
	while(input){
		fflush(stdout);
		char choice = getchar();
		fflush(stdin);
		choice = toupper(choice);
		int direction = choice - 'A';
		if(direction >= 0 && direction < MAX_DIR){
			if(c->entrancesizes[direction]>0){
				travelRoute(c->routes[direction],c);
				input = 0;
				return;
			}else{
				printf("Exit not present. Try another.\n");
			}
		}else if(choice=='Q'){
			return;
		}else{
			printf("Not an exit. Try again.\n");
		}
	}
}
void travelRoute(route* r,cave *source){
	printRoute(r);
	printf("\n");
	printf("A: Continue\n");
	printf("B: Go back\n");

	int input = 1;
	while(input){
		fflush(stdout);
		char choice = getchar();
		fflush(stdin);
		choice = toupper(choice);

		switch(choice){
			case 'A' :
				if(r->cavea==source){
					travelCave(r->caveb);
				}else{
					travelCave(r->cavea);
				}
				return;
			case 'B' :
				if(r->cavea==source){
					travelCave(r->cavea);
				}else{
					travelCave(r->caveb);
				}
				return;
			default:
				printf("Not a valid choice. Try again.\n");
				break;
		}
	}
}