#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAVES 10
#define MAX_DIR 6
#define MAX_ROUTES (MAX_CAVES*MAX_DIR)

//enum directions {north,east,south,west,up,down};
char dir[6][6];

struct cave;
struct route;
struct cavesystem;

typedef struct cave cave;
typedef struct route route;
typedef struct cavesystem cavesystem;

int d(int);
int range(int,int);

cave* generateCave();
cavesystem* generateCaveSystem();
void printCave(cave*);

struct cave{
	int cavesize;
	route *routes[MAX_DIR];
	int entrancesizes[MAX_DIR];
};
struct route{
	//
	int a;
	//
};
struct cavesystem{
	cave* caves;
	route* routes;
	int numcaves;
	int numroutes;
};

int main(){
	srand(time(NULL));
	strcpy(dir[0],"North");
	strcpy(dir[1],"East");
	strcpy(dir[2],"South");
	strcpy(dir[3],"West");
	strcpy(dir[4],"Up");
	strcpy(dir[5],"Down");

	
	printf("Welcome to the Veins of the Earth semi-autonomout GM.\n");
	printf("Please select an option.\n");
	printf("A: Generate a cave system\n");
	printf("B: Read a cave system\n");
	fflush(stdout);

	int input = getchar();
	input = toupper(input);

	switch(input){
		case 'A' :
			;//just go with it
			cave *gen = generateCave();
			printCave(gen);
			free(gen);
			break;
		case 'B' :
			//read cave
			break;
	}

}

cavesystem* generateCaveSystem(){
	cavesystem* csys = malloc(sizeof(cavesystem));
	csys->caves = malloc(sizeof(cave*) * MAX_CAVES);
	csys->routes = malloc(sizeof(route*) * MAX_ROUTES);

	for(int i = 0; i < MAX_CAVES; i++){

	}
}

cave* generateCave(){
	cave* togen = malloc(sizeof(cave));
	
	for(int i=0;i<MAX_DIR;i++){
		togen->routes[i] = NULL;
		togen->entrancesizes[i] = 0;
	}

	int size = d(8);
	switch(size){
		case 1:
			togen->cavesize = range(2,6);
			break;
		case 2:
			togen->cavesize = range(7,15);
			break;
		case 3:
			togen->cavesize = range(16,25);
			break;
		case 4:
			togen->cavesize = range(26,35);
			break;
		case 5:
			togen->cavesize = range(36,45);
			break;
		case 6:
			togen->cavesize = range(46,60);
			break;
		case 7:
			togen->cavesize = range(61,500);
			break;
		case 8:
			break;
			//gargantuan
	}

	int roomsum = d(6)+d(6);
	while(roomsum>1){
		int wall = d(MAX_DIR)-1;
		while(togen->routes[wall] != NULL){
			wall = d(MAX_DIR)-1;
		}
		togen->routes[wall] = malloc(sizeof(route));
		togen->entrancesizes[wall] = roomsum;
		roomsum = roomsum/2;
	}

	return togen;
}

int d(int diesize){
	return rand()%diesize + 1;
}
int range(int start, int end){
	return start + rand()%(end-start);
}
void printCave(cave* c){
	printf("Cave Size: %d feet\n",c->cavesize);
	for(int i = 0; i < MAX_DIR; i++){
		if(c->entrancesizes[i]>0){
			printf("Exit %s Size: %d feet\n",dir[i],c->entrancesizes[i]);
		}
	}
}