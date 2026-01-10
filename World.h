//
// Created by jakub on 06/01/2026.
//

#ifndef _WORLD_H
#define _WORLD_H
#include "Walker.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Square.h"
#include "Walker.h"

#define MAX_SIZE 100

typedef enum {
    Obstacle,
    NonObstacle

}WorldType;


typedef struct World {
    Walker *walker;
    int size;
    int replications;
    int mode;
    Square **square;
    WorldType type;
}World;

_Bool try_openning(char * str);
void new_simulation(World * world);
void connect(World * world);
void rerun(World * world);

#endif //_WORLD_H