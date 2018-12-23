#ifndef GENERATION
#define GENERATION

#include "cavestructures.h"

#define MAX_CAVES 10
#define MAX_ROUTES (MAX_CAVES*MAX_DIR)
#define MIN_ROUTES (MAX_CAVES+MAX_CAVES/4)
#define MAX_LOOSE 5

cave* generateCave();
cavesystem* generateCaveSystem();
cavesystem* generateCaveSystemInterior();
route* generateRoute(cave*,cave*,int,int);


void destroyCaveSystem(cavesystem*);

int getexit(cave*);
int looseexits(cavesystem*);
int connected(cavesystem*);
int traversal(cave*,int);

#endif