/*Grupo: Stéphane Ammar ra149071
 * 	 Andréia Yukie Uratsuka 096952 
 * Animacao referente ao Projeto 02 da materia MC504 com a professora Islene
 * referente ao problema : The Dining Savages
 * Data 22/04/2014
*/
#include <semaphore.h>
#include "savage.h"

struct Savage * _cooker;
struct Pot * _pot;

void * cookerLife (void *);
void * savageLife (void *);

void walk (struct Savage *, int, int,int);
void eat  (struct Savage *);
void wander  (struct Savage *);

int numTotSavages;

//sem_t *semaphores;

int *waitNeigh;

pthread_mutex_t emptyPot = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fullPot = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Starts the pot and the cooker with positions and meals
 *
 * potX {int} X coordinate of the pot
 * potY {int} Y coordinate of the pot
 * potCapacity {int} maximum meals in the pot
 * cookerX {int} X coordinate of the cooker
 * cookerY {int} Y coordinate of the cooker
 * numSavages {int} : number of savages.
 */
void start (int potX, int potY, int potCapacity, int cookerX, int cookerY, int numSavages) {
    pthread_t thr;
	int i;
	numTotSavages = numSavages;

    _cooker         = (struct Savage *) malloc(sizeof(struct Savage));
    _cooker->status = SLEEPING;
    _cooker->x      = cookerX;
    _cooker->y      = cookerY;

    _pot            = (struct Pot *) malloc(sizeof(struct Pot));
    _pot->capacity  = potCapacity;
    _pot->meals     = potCapacity;
    _pot->x         = potX;
    _pot->y         = potY;

	//semaphores = malloc((numSavages)*sizeof(sem_t));
	waitNeigh=malloc((numSavages)*sizeof(int));
	for(i=0;i<numSavages;i++){
	//sem_init(&semaphores[i], 0, 1);
	waitNeigh[i]=1;
	}

    pthread_mutex_lock(&emptyPot);
    pthread_create(&thr, NULL, cookerLife, (void *) _cooker);
}

/*
 * Creates a new savage
 *
 * x {int} X coordinate of the savage
 * y {int} Y coordinate of the savage
 */
struct Savage * newSavage (int x, int y, int position) {
    struct Savage * savage;
    pthread_t thr;

    savage         = (struct Savage *) malloc(sizeof(struct Savage));
    savage->status = WANDER;
    savage->x      = x;
    savage->y      = y;
	savage->position = position;
    pthread_create(&thr, NULL, savageLife, (void *) savage);

    return savage;
}

/*
 * Returns the cooker
 */
struct Savage * cooker () {
    return _cooker;
}

/*
 * Returns the pot
 */
struct Pot * pot () {
    return _pot;
}

/*
 * Controls the cooker thread
 *
 * v {void *} pointer to the cooker
 */
void * cookerLife (void * v) {
    struct Savage * cooker = (struct Savage *) v;
    int originalX = cooker->x;
    int originalY = cooker->y;

    while (1) {
        cooker->status = SLEEPING;
        pthread_mutex_lock(&emptyPot);
        walk(cooker, _pot->x, _pot->y,0);
        _pot->meals = _pot->capacity;
        pthread_mutex_unlock(&fullPot);
        walk(cooker, originalX, originalY,0);
    }
}

/*
 * Controls the savage thread
 *
 * v {void *} pointer to the savage
 */
void * savageLife (void * v) {
    struct Savage * savage = (struct Savage *) v;
    int originalX = savage->x;
    int originalY = savage->y;

    while (1) {
	wander(savage);
        pthread_mutex_lock(&mutex);
        if (_pot->meals <= 0) {
            pthread_mutex_unlock(&emptyPot);
            pthread_mutex_lock(&fullPot);
        }
	//sem_wait(&semaphores[savage->position]);
	waitNeigh[savage->position]=0;
        walk(savage, _pot->x, _pot->y,0);
        _pot->meals--;
		pthread_mutex_unlock(&mutex);
        walk(savage, originalX, originalY,1); 
	//sem_post(&semaphores[savage->position]);    
	waitNeigh[savage->position]=1;
        eat(savage);
    }
}

/*
 * Makes the savage walks from the current position to a new position
 *
 * savage {struct Savage *} pointer to the savage
 * finalX {int} final X coordinate after walking
 * finalY {int} final Y coordinate after walking
 * veryHungry {int} : 0 pra caminhar devagar, 1 señao (o savage vai mais rapido uma vez que ele tem comida
 *                    pois ele tá com muita fome
 */
void walk (struct Savage * savage, int finalX, int finalY, int veryHungry) {
    int xIncrement = savage->x < finalX ? 1 : -1;
    int yIncrement = savage->y < finalY ? 1 : -1;
    int espera=200000;
	if(veryHungry)
		espera=100000;
    savage->status = WALKING;
    while (!(savage->x == finalX && savage->y == finalY)) {
        if (!(savage->x == finalX)) savage->x += xIncrement;
        if (!(savage->y == finalY)) savage->y += yIncrement;
        usleep(espera);
    }
}

/*
 * Eat some meal from the pot
 *
 * savage {struct Savage *} pointer to the savage
 */
void eat (struct Savage * savage) {
int position = savage->position;
	if(position==numTotSavages-1){
		//sem_wait(&semaphores[position+1]);
		while(!waitNeigh[position-1])
			savage->status = WAITINGNEIGHBOR;} 
else{
	if(position==0)	{
		//sem_wait(&semaphores[position-1]);
		while(!waitNeigh[position+1])
			savage->status = WAITINGNEIGHBOR;}
	else{
	while(!waitNeigh[position+1] || !waitNeigh[position-1])
		savage->status = WAITINGNEIGHBOR;
}}
    savage->status = EATING;
    usleep(5000000 + 1000000 * (rand() % 10));
	
	//savage->status = HUNGRY;
}

void wander(struct Savage * savage){
	savage->status = WANDER;
	usleep(1000000 * (rand() % 10));
	savage ->status = HUNGRY;
	}
