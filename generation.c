#include "generation.h"

#include "veinsutil.h"
#include "cavestructures.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>

//Generate entire cave system
cavesystem* generateCaveSystem(){
	cavesystem *csys = NULL;
	while(csys==NULL){
		csys = generateCaveSystemInterior();
		if(!connected(csys) || csys->numroutes<MIN_ROUTES){
			destroyCaveSystem(csys);
			csys=NULL;
		}
	}
	return csys;
}
//Interior function. Generate cave system to be judged
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