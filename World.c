//
// Created by jakub on 06/01/2026.
//

#include "World.h"
#include "Walker.h"




_Bool try_openning(char * str) {
    FILE *f = fopen(str, "r");
    if (f == NULL) {
        printf("File not found!\n");
        return 0;
    }
    return 1;
}

void new_simulation(World * world) {

}

void connect(World * world) {


}

void rerun(World * world) {

}

int chceckForObstacle(World * world, int row, int col) {
    if (world->square[row][col].representation == Obstacle) {
        return 0;
    }
    return 1;
}

/*int main() {
    World world;
    initWorld(&world);
    new_simulation(&world);
    return 0;
}*/