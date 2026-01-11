#include "Square.h"


void initSquare(Square * square, _Bool hasObstacles) {

    int ran = rand() % 101;
    if (ran <= 10 && hasObstacles) {
        square->representation = Barrier;
    } else {
        square->representation = Empty;
    }
}

void set_finish(Square * square) {
    square->representation = Finish;
}

_Bool is_finished(Square * square) {
    return square->representation == Finish;
}

