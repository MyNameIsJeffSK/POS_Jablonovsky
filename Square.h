//
// Created by jakub on 07/01/2026.
//

#ifndef SEMESTRALKA_SQUARE_H
#define SEMESTRALKA_SQUARE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef enum {
    Empty = '.',
    Finish = 'X',
    Barrier = '#'
}Representation;

typedef struct {
    Representation representation;
    _Bool successful_attempts;
    int value;
    double priemer_posunov;
}Square;

void initSquare(Square * square, _Bool hasObstacles);
void set_finish(Square * square);
void is_finish(Square * square);

#endif //SEMESTRALKA_SQUARE_H