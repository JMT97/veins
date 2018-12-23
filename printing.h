#ifndef PRINTING
#define PRINTING

#include "cavestructures.h"

void printCave(cave*);
void printRoute(route*);
void printRouteShort(route*);
void printCaveSystem(cavesystem*);

void travelCave(cave*);
void travelRoute(route*,cave*);

#endif