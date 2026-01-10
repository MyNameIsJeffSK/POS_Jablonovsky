//
// Created by jakub on 06/01/2026.
//

#ifndef SEMESTRALKA_WALKER_H
#define SEMESTRALKA_WALKER_H
#include <stdio.h>
#include <time.h>


typedef struct World World;

typedef enum {
    Up, Down, Left, Right
}Direction;

typedef struct {
    int pos_x;
    int pos_y;
    int steps;
    int steps_done;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    struct World * world;
}Walker;



Direction getDirection(int num, Walker * walker);

#endif //SEMESTRALKA_WALKER_H

