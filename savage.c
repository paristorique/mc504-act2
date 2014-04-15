/*Grupo: Bruno Vargas    ra 116330
 *       Jonatan Valongo ra 117424
 *       Rafael Erthal   ra 121286
 * Animacao referente ao Projeto 01 da materia MC504 com a professora Islene
 * referente ao problema : The Dining Savages
 * Data 27/09/2013
*/

#include "savage.h"

struct Savage * _cooker;
struct Pot * _pot;

void * cookerLife (void *);
void * savageLife (void *);

void walk (struct Savage *, int, int);
void eat  (struct Savage *);
void wander  (struct Savage *);

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
 */
void start (int potX, int potY, int potCapacity, int cookerX, int cookerY) {
    pthread_t thr;

    _cooker         = (struct Savage *) malloc(sizeof(struct Savage));
    _cooker->status = SLEEPING;
    _cooker->x      = cookerX;
    _cooker->y      = cookerY;

    _pot            = (struct Pot *) malloc(sizeof(struct Pot));
    _pot->capacity  = potCapacity;
    _pot->meals     = potCapacity;
    _pot->x         = potX;
    _pot->y         = potY;

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
        walk(cooker, _pot->x, _pot->y);
        _pot->meals = _pot->capacity;
        pthread_mutex_unlock(&fullPot);
        walk(cooker, originalX, originalY);
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
        walk(savage, _pot->x, _pot->y);
        _pot->meals--;
		pthread_mutex_unlock(&mutex);
        walk(savage, originalX, originalY);     
        eat(savage);
    }
}

/*
 * Makes the savage walks from the current position to a new position
 *
 * savage {struct Savage *} pointer to the savage
 * finalX {int} final X coordinate after walking
 * finalY {int} final Y coordinate after walking
 */
void walk (struct Savage * savage, int finalX, int finalY) {
    int xIncrement = savage->x < finalX ? 1 : -1;
    int yIncrement = savage->y < finalY ? 1 : -1;

    savage->status = WALKING;
    while (!(savage->x == finalX && savage->y == finalY)) {
        if (!(savage->x == finalX)) savage->x += xIncrement;
        if (!(savage->y == finalY)) savage->y += yIncrement;
        usleep(150000);
    }
}

/*
 * Eat some meal from the pot
 *
 * savage {struct Savage *} pointer to the savage
 */
void eat (struct Savage * savage) {
    savage->status = EATING;
    usleep(5000000 + 1000000 * (rand() % 10));
	
	//savage->status = HUNGRY;
}

void wander(struct Savage * savage){
	savage->status = WANDER;
	usleep(1000000 * (rand() % 10));
	savage ->status = HUNGRY;
	}
