#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SLEEPING 0
#define HUNGRY 1
#define EATING 2
#define WALKING 3
#define WANDER 4

struct Pot {
    int meals;
    int capacity;
    int x;
    int y;
};

struct Savage {
    int status;
    int x;
    int y;
	int position;
};

struct Pot * pot ();
struct Savage * cooker ();
struct Savage * newSavage (int,int,int);
void start (int,int,int,int,int);
