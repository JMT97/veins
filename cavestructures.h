#ifndef CAVESTRUCTS
#define CAVESTRUCTS

#include "veinsutil.h"

struct cave;
struct route;
struct cavesystem;

typedef struct cave cave;
typedef struct route route;
typedef struct cavesystem cavesystem;

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

#endif