#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAVES 10
#define MAX_DIR 6
#define MAX_ROUTES (MAX_CAVES*MAX_DIR)

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
route* generateRoute(cave*,cave*,int,int);
void printCave(cave*);
void printRoute(route*);

struct cave{
	char sizedescription[100];
	int cavesize;
	route *routes[MAX_DIR];
	int entrancesizes[MAX_DIR];
};
struct route{
	//
	int time;
	char routetype[100];
	cave* cavea;
	cave* caveb;
	//
};
struct cavesystem{
	cave* caves;
	route* routes;
	int numcaves;
	int numroutes;
};

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
			printCave(gen);

			route *gen3 = generateRoute(gen,gen2,0,0);
			printRoute(gen3);
			
			free(gen);
			free(gen2);
			free(gen3);
			break;
		case 'B' :
			//read cave
			break;
	}

}

//TODO
cavesystem* generateCaveSystem(){
	cavesystem* csys = malloc(sizeof(cavesystem));
	csys->caves = malloc(sizeof(cave*) * MAX_CAVES);
	csys->routes = malloc(sizeof(route*) * MAX_ROUTES);

	for(int i = 0; i < MAX_CAVES; i++){

	}
	
}



//Generate one cave and cave exits.
cave* generateCave(){
	//Initialization
	cave* togen = malloc(sizeof(cave));
	for(int i=0;i<MAX_DIR;i++){
		togen->routes[i] = NULL;
		togen->entrancesizes[i] = 0;
	}

	//Generate the cave size
	int size = d(8);
	switch(size){
		case 1:
			strcpy(togen->sizedescription,"A tiny cave, perhaps the size of a cupboard. No more than a junction in the route.");
			togen->cavesize = range(2,6);
			break;
		case 2:
			strcpy(togen->sizedescription,"Room sized.");
			togen->cavesize = range(7,15);
			break;
		case 3:
			strcpy(togen->sizedescription,"The size of two rooms.");
			togen->cavesize = range(16,25);
			break;
		case 4:
			strcpy(togen->sizedescription,"The size of three rooms.");
			togen->cavesize = range(26,35);
			break;
		case 5:
			strcpy(togen->sizedescription,"The size of four rooms.");
			togen->cavesize = range(36,45);
			break;
		case 6:
			strcpy(togen->sizedescription,"The size of a house.");
			togen->cavesize = range(46,60);
			break;
		case 7:
			strcpy(togen->sizedescription,"The size of an office building.");
			togen->cavesize = range(61,500);
			break;
		case 8:
			strcpy(togen->sizedescription,"A gargantuan cave.");
			break;
			//gargantuan
	}

	//Generate exits
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

route* generateRoute(cave* c1, cave* c2, int exitA, int exitB){
	//create route
	route* togen = malloc(sizeof(route));
	//add caves
	togen->cavea = c1;
	togen->caveb = c2;
	//add this route to the caves in the indicies

	c1->routes[exitA] = togen;
	c2->routes[exitB] = togen;
	//TODO: DO WE WANT TO ADD WHICH ENTRANCE THIS ROUTE IS IN THE STRUCT? IE EXIT A OR EXIT B

	//Get length of the route
	int time = d(4);
	if(time == 4){
		int deltatime = d(6);
		time += deltatime;
		if(deltatime == 6){
			deltatime = d(8);
			time += deltatime;
			if(deltatime == 8){
				deltatime = d(10);
				time += deltatime;
				if(deltatime == 10){
					deltatime = d(12);
					time += deltatime;
					if(deltatime == 12){
						do{
							deltatime = d(20);
							time += deltatime;
						}while(deltatime==20);
					}
				}
			}
		}
	}
	togen->time=time;

	//Generate type of route
	int type = d(4);
	switch(type){
		case 1:
			strcpy(togen->routetype,"This route is a walk or shaft.");
			break;
		case 2:
			strcpy(togen->routetype,"This route is a crawl or chimney.");
			break;
		case 3:
			strcpy(togen->routetype,"This route is a squeeze.");
			break;
		case 4:
			type = d(4);
			switch(type){
				case 1:
					strcpy(togen->routetype,"Traverse: The floor here is dangerous.");
					break;
				case 2:
					strcpy(togen->routetype,"Sump. A river or pool of water.");
					break;
				case 3:
					strcpy(togen->routetype,"Letterbox Squeeze. A very extreme squeeze.");
					break;
				case 4:
					strcpy(togen->routetype,"Breakdown. This route is blocked by fallen rocks.");
					break;
			}
			break;
	}

	//return the route
	return togen;
}


//Is just a die
int d(int diesize){
	return rand()%diesize + 1;
}
//This is for being lazy and generating numbers in a range (inclusive,inclusive)
int range(int start, int end){
	return start + rand()%(end-start+1);
}
//This prints the information for a single cave
void printCave(cave* c){
	printf("\n\n");
	printf("%s\n",c->sizedescription);
	printf("Cave Size: %d feet\n",c->cavesize);
	for(int i = 0; i < MAX_DIR; i++){
		if(c->entrancesizes[i]>0){
			printf("Exit %s Size: %d feet\n",dir[i],c->entrancesizes[i]);

		}
	}
	fflush(stdout);
}
//This prints the information for a single route
void printRoute(route* r){
	printf("\n\n");
	printf("%s\n",r->routetype);
	printf("Route Length: %d minutes\n",r->time*10);
	fflush(stdout);
}