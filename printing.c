#include "printing.h"

#include "veinsutil.h"
#include "cavestructures.h"

#include <stdio.h>

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
		//printCave(csys->caves[i]);
	}
	for(int i = 0; i < csys->numroutes; i++){
		printRouteShort(csys->routes[i]);
	}
}