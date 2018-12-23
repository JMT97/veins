#include "veinsutil.h"

#include <stdlib.h>

//Is just a die
int d(int diesize){
	return rand()%diesize + 1;
}
//This is for being lazy and generating numbers in a range (inclusive,inclusive)
int range(int start, int end){
	return start + rand()%(end-start+1);
}