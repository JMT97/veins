#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CAVES 10
#define MAX_DIR 6
#define MAX_ROUTES (MAX_CAVES*MAX_DIR)
#define MIN_ROUTES (MAX_CAVES+MAX_CAVES/4)
#define MAX_LOOSE 5

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
cavesystem* generateCaveSystemInterior();
route* generateRoute(cave*,cave*,int,int);
int getexit(cave*);
int looseexits(cavesystem*);
int connected(cavesystem*);
int traversal(cave*,int);
void destroyCaveSystem(cavesystem*);
void printCave(cave*);
void printRoute(route*);
void printRouteShort(route*);
void printCaveSystem(cavesystem*);

struct cave{
	int caveID;
	char sizedescription[100];
	int cavesize;
	route *routes[MAX_DIR];
	int entrancesizes[MAX_DIR];
};
struct route{
	//
	int routeID;
	int time;
	char routetype[100];
	cave* cavea;
	cave* caveb;
	//
};
struct cavesystem{
	cave** caves;
	route** routes;
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

cavesystem* generateCaveSystem(){
	cavesystem *csys = NULL;
	while(csys==NULL){
		csys = generateCaveSystemInterior();
		if(!connected(csys) || csys->numroutes<MIN_ROUTES){
			printf("Cave system not connected.\n");
			fflush(stdout);
			destroyCaveSystem(csys);
			csys=NULL;
		}
	}
	return csys;
}
cavesystem* generateCaveSystemInterior(){
	cavesystem* csys = malloc(sizeof(cavesystem));
	csys->numcaves = 0;
	csys->numroutes = 0;
	csys->caves = malloc(sizeof(cave*) * MAX_CAVES);
	csys->routes = malloc(sizeof(route*) * MAX_ROUTES);

	for(int i = 0;i<MAX_CAVES;i++){
		csys->caves[i] = generateCave();
		csys->caves[i]->caveID = i;
		csys->numcaves++;
	}

	int x[MAX_CAVES];
	int y[MAX_CAVES];
	int z[MAX_CAVES];
	for(int i = 0; i < MAX_CAVES; i++){
		x[i] = d(100);
		y[i] = d(100);
		z[i] = d(100);
	}
	double distances[MAX_CAVES][MAX_CAVES];
	for(int i = 0; i < MAX_CAVES; i++){
		for(int p = 0; p < MAX_CAVES; p++){
			double delx = (double) x[i] - x[p];
			double dely = y[i]-y[p];
			double delz = z[i]-z[p];
			distances[i][p] = sqrt(delx*delx + dely*dely + delz*delz);
		}
	}

	while(looseexits(csys)>MAX_LOOSE){
	//for(int i = 0; i < NUM_ROUTES; i++){
		int cave1 = d(MAX_CAVES)-1;
		int cave2 = d(MAX_CAVES)-1;
		cave *c1 = csys->caves[cave1];
		cave *c2 = csys->caves[cave2];
		int dir1 = getexit(c1);
		int dir2 = getexit(c2);


		fflush(stdout);

		if(dir1!=-1 && dir2!=-1){
			if( d(100) < (100*sqrt(3) - distances[cave1][cave2]) ){
				if(c1!=c2 || dir1!=dir2){
					route* r = generateRoute(c1,c2,getexit(c1),getexit(c2));
					r->routeID = csys->numroutes;
					csys->routes[csys->numroutes] = r;
					csys->numroutes++;
				}
			}
		}
	}

	//Pruning Loose exits
	for(int i = 0; i < csys->numcaves; i++){
		cave *c = csys->caves[i];
		if(c!=NULL){
			for(int e = 0;e<MAX_DIR;e++){
				if(c->entrancesizes[e]>0 && c->routes[e]==NULL){
					c->entrancesizes[e]=0;
				}
			}
		}
	}

	return csys;
}
void destroyCaveSystem(cavesystem* csys){
	for(int i = 0; i < csys->numcaves; i++){
		if(csys->caves[i]!=NULL){
			free(csys->caves[i]);
		}
	}
	for(int i = 0; i < csys->numroutes; i++){
		if(csys->routes[i]!=NULL){
			free(csys->routes[i]);
		}
	}
	free(csys);
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
		while(togen->entrancesizes[wall] != 0){
			wall = d(MAX_DIR)-1;
		}
		//togen->routes[wall] = NULL;
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
//finds a random exit to a cave if one exists and returns -1 otherwise
int getexit(cave *c){
	int startdir = d(MAX_DIR);
	for(int i = 0; i < MAX_DIR; i++){
		int dir = (startdir + i)%MAX_DIR;
		if(c->entrancesizes[dir] != 0 && c->routes[dir]==NULL){
			return dir;
		}
	}
	return -1;
}
//Finds the number of loose entrances
int looseexits(cavesystem* csys){
	int exits = 0;
	for(int i = 0; i < csys->numcaves; i++){
		cave *c = csys->caves[i];
		if(c!=NULL){
			for(int e = 0;e<MAX_DIR;e++){
				if(c->entrancesizes[e]>0 && c->routes[e]==NULL){
					exits++;
				}
			}
		}
	}
	return exits;
}
//Finds the other end of a tunnel
int traversal(cave *c, int dir){
	//Check that route exists
	if(c->routes[dir]==NULL){
		return -1;
	}
	//Find other end of tunnel
	route *r = c->routes[dir];
	if(r->cavea!=NULL && r->cavea != c){
		return r->cavea->caveID;
	}
	else if(r->caveb!=NULL){
		return r->caveb->caveID;
	}
	return -1;
}
//Check if a cave system is fully connected
int connected(cavesystem* csys){
	int present[MAX_CAVES];
	int visited[MAX_CAVES];
	int fringe[MAX_CAVES];
	for(int i = 0; i < csys->numcaves; i++){
		if(csys->caves[i]!=NULL){
			present[i]=1;
			visited[i]=0;
		}
		else{
			present[i]=0;
			visited[i]=1;
		}
		fringe[i] = 0;
	}
	//Traverse the cave system as a tree, marking all caves visited
	fringe[0] = 1;//Technically should check for present
	while(1){
		//check fringe
		int isone = 0;
		//Do the tree traversal
		for(int i = 0; i < MAX_CAVES; i++){
			if(present[i] && fringe[i]){
				cave *c = csys->caves[i];
				isone = 1;
				fringe[i] = 0;
				visited[i] = 1;
				for(int dir = 0; dir < 6; dir++){
					int nextcave = traversal(c,dir);
					if(present[nextcave] && !visited[nextcave] && !fringe[nextcave]){
						fringe[nextcave] = 1;
					}
				}
			}
		}
		//Check if the traversal is complete
		if(!isone){
			break;
		}
	}
	//Check to see if any caves are not linked
	int connected = 1;
	for(int i = 0; i < MAX_CAVES; i++){
		if(present[i] && !visited[i]){
			connected = 0;
		}
	}

	return connected;
}
//This prints the information for a single cave
void printCave(cave* c){
	printf("\n\n");
	printf("Cave %d.\n",c->caveID);
	printf("%s\n",c->sizedescription);
	printf("Cave Size: %d feet\n",c->cavesize);
	for(int i = 0; i < MAX_DIR; i++){
		if(c->entrancesizes[i]>0){
			if(c->routes[i]!=NULL){
				printf("Exit %s Size: %d feet. Leads to route %d\n",dir[i],c->entrancesizes[i],c->routes[i]->routeID);
			}
			else{
				printf("Exit %s Size: %d feet\n",dir[i],c->entrancesizes[i]);
			}
		}
	}
	fflush(stdout);
}
//This prints the information for a single route
void printRoute(route* r){
	printf("\n\n");
	printf("Route %d. From cave %d to cave %d.\n",r->routeID,r->cavea->caveID,r->caveb->caveID);
	printf("%s\n",r->routetype);
	printf("Route Length: %d minutes\n",r->time*10);
	fflush(stdout);
}
//This prints Just the routing information for a route
void printRouteShort(route* r){\
	printf("%d to %d\n",r->cavea->caveID,r->caveb->caveID);
	fflush(stdout);
}
//This prints the information for an entire cave system
void printCaveSystem(cavesystem* csys){
	printf("\n\nCave system has %d caves and %d routes.\n",csys->numcaves,csys->numroutes);
	for(int i = 0; i < csys->numcaves; i++){
		printCave(csys->caves[i]);
	}
	for(int i = 0; i < csys->numroutes; i++){
		printRoute(csys->routes[i]);
	}
}