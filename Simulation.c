#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Simulation.h"

void init_World(World * world) {
    world->size = 0;
    world->replications = 0;
    world->mode = 0;
    world->square = NULL;
    world->walker = NULL;
}



/*void calculate_square(World * world) {

    int i = 0;
    int ran = 0;
    for (int i = 0; i < world->size; i++) {
        for (int j = 0; j < world->size; j++) {
            world->walker->pos_x = i;
            world->walker->pos_y = j;
            world->walker->steps_done = 0;
            while (i < world->walker->steps) {
                ran = (rand() % 100) + 1;
                switch (getDirection(ran, world->walker)) {//
                    case Up:
                        if (world->walker->pos_y - 1 < 0) {// overflow
                            if (world->square[world->walker->pos_x][world->size - 1].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_y = world->size - 1;
                            }
                        } else {
                            if (world->square[world->walker->pos_x][j - 1].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_y--;
                            }
                        }
                        break;
                    case Down:
                        if ((world->walker->pos_y) + 1 > world->size - 1) {// overflow
                            if (world->square[world->walker->pos_x][0].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_y = 0;
                            }
                        } else {
                            if (world->square[world->walker->pos_x][world->walker->pos_y + 1].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_y--;
                            }
                        }
                        break;
                    case Left:
                        if ((world->walker->pos_x) - 1 < 0) {// overflow
                            if (world->square[world->size - 1][world->walker->pos_y].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_x = world->size - 1;
                            }
                        } else {
                            if (world->square[world->size - 1][world->walker->pos_y].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_x = world->size - 1;
                            }
                        }
                        break;
                    case Right:
                        if ((world->walker->pos_x) + 1 > world->size - 1) {// overflow
                            if (world->square[0][world->walker->pos_y].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_x = 0;
                            }
                        } else {
                            if (world->square[world->walker->pos_x + 1][world->walker->pos_y].representation == '#') {
                                world->walker->steps_done++;
                            } else {
                                world->walker->steps_done++;
                                world->walker->pos_x++;
                            }
                        }
                        break;
                }
                if (world->walker->pos_y == 0 && world->walker->pos_x == 0) {
                    break;
                }
                i++;
            }
            world->square[i][j].value = world->walker->steps_done;
        }
    }

}*/

void calculate_square(World * world, int start_x, int start_y) {
    if (start_x == 0 && start_y == 0) return;
   
    int original_x = world->walker->pos_x;

    int original_y = world->walker->pos_y;
    world->walker->pos_x = start_x;
    world->walker->pos_y = start_y;
    world->walker->steps_done = 0;

    int steps_taken = 0;
    while (steps_taken < world->walker->steps) {
        // Generate random number 0-99 for probability comparison
        int rand_num = rand() % 100;
        Direction dir = getDirection(rand_num, world->walker);

        int new_x = world->walker->pos_x;
        int new_y = world->walker->pos_y;

        switch (dir) {
            case Up:    new_x--; break;
            case Down:  new_x++; break;
            case Left:  new_y--; break;
            case Right: new_y++; break;
        }

        if (new_x < 0) new_x = world->size - 1;
        else if (new_x >= world->size) new_x = 0;

        if (new_y < 0) new_y = world->size - 1;
        else if (new_y >= world->size) new_y = 0;

        if (world->square[new_x][new_y].representation == '#') {
            world->walker->steps_done++;
        } else {
            world->walker->pos_x = new_x;
            world->walker->pos_y = new_y;
            world->walker->steps_done++;
        }

        if (world->walker->pos_x == 0 && world->walker->pos_y == 0) {
            world->square[start_x][start_y].successful_attempts = 1;
            break;
        }

        steps_taken++;
    }

    world->square[start_x][start_y].value = world->walker->steps_done;
    world->walker->pos_x = original_x;
    world->walker->pos_y = original_y;
}

void calculate_World_summary(World * world) {
    for (int i = 0; i < world->size; i++) {
        for (int j = 0; j < world->size; j++) {
            world->square[i][j].value = 0;
            world->square[i][j].successful_attempts = 0;
            if ((i == 0 && j == 0) || world->square[i][j].representation == '#') {
                continue;
            }
            calculate_square(world, i, j);
        }
    }
}

void get_info(Simulation * simulation) {
    

    for (int i = 0; i < simulation->replications; i++) {

        simulation->world[i].replications = simulation->replications;
        simulation->world[i].size = simulation->size;
        simulation->world[i].type = simulation->type;
        simulation->world[i].mode = simulation->mode;

        simulation->world[i].walker = malloc(sizeof(Walker));
        if (simulation->world[i].walker == NULL) {
            printf("Memory allocation failed for walker!\n");
            return;
        }
        simulation->world[i].walker->steps = simulation->steps;
        simulation->world[i].walker->steps_done = 0;
        simulation->world[i].walker->up_p = simulation->up_p;
        simulation->world[i].walker->down_p = simulation->down_p;
        simulation->world[i].walker->left_p = simulation->left_p;
        simulation->world[i].walker->right_p = simulation->right_p;
        simulation->world[i].walker->pos_x = 0;
        simulation->world[i].walker->pos_y = 0;
        simulation->world[i].walker->world = &simulation->world[i];

        simulation->world[i].square = malloc(simulation->size * sizeof(Square *)); // column
        if (simulation->world[i].square == NULL) {
            printf("Memory allocation failed for square rows!\n");
            return;
        }

        for (int row = 0; row < simulation->size; row++) { // row
            simulation->world[i].square[row] = malloc(simulation->size * sizeof(Square));
            if (simulation->world[i].square[row] == NULL) {
                printf("Memory allocation failed for square columns!\n");
                return;
            }
        }

        if (i == 0) {// generate first world
            for (int row = 0; row < simulation->size; row++) {
                for (int col = 0; col < simulation->size; col++) {
                    if (row == 0 && col == 0) {
                        set_finish(&simulation->world[i].square[row][col]);
                    } else {
                        initSquare(&simulation->world[i].square[row][col],
                                  simulation->type == Obstacle);
                    }
                }
            }
        } else {// copy grid to all the other worlds
            for (int row = 0; row < simulation->size; row++) {
                for (int col = 0; col < simulation->size; col++) {
                    simulation->world[i].square[row][col].representation =
                        simulation->world[0].square[row][col].representation;
                }
            }
        }
        calculate_World_summary(&simulation->world[i]);
    }




    for (int row = 0; row < simulation->size; row++) {// display normal grid
        for (int col = 0; col < simulation->size; col++) {
            printf(" %c ", simulation->world[0].square[row][col].representation);
        }
        printf("\n");
    }
    printf("\n");


    /*for (int rep = 0; rep < replications; rep++) {// Display
        printf("\n=== World %d ===\n", rep + 1);
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if ((row == 0 && col == 0 )|| simulation->world[rep].square[row][col].representation == '#') {
                    printf(" %c ", simulation->world[rep].square[row][col].representation);
                } else {
                    printf(" %d ", simulation->world[rep].square[row][col].value);
                }
            }
            printf("\n");
        }
        printf("\n");
    }*/
    printf("======Results=====\n");
    printf("Average steps:\n");
    double result = 0;
    for (int row = 0; row < simulation->size; row++) {// display average steps
        for (int col = 0; col < simulation->size; col++) {
            for (int rep = 0; rep < simulation->replications; rep++) {
                result += simulation->world[rep].square[row][col].value;
            }
            if ((row == 0 && col == 0 )|| simulation->world[0].square[row][col].representation == '#') {
                printf("%c\t", simulation->world[0].square[row][col].representation);
            } else {
                result = (double)result / (double)simulation->replications;
                printf("%.1lf\t", result);
                result = 0;
            }
        }
        printf("\n");
    }

    printf("Probability of reaching X:\n");
    for (int row = 0; row < simulation->size; row++) {// display probability
        for (int col = 0; col < simulation->size; col++) {
            double num = 0;
            for (int rep = 0; rep < simulation->replications; rep++) {
                if (simulation->world[rep].square[row][col].successful_attempts == 1) {
                    num++;
                }
            }
            if ((row == 0 && col == 0 )|| simulation->world[0].square[row][col].representation == '#') {
                printf("%c\t", simulation->world[0].square[row][col].representation);
            } else {
                num = ((double)num * 100) / (double)simulation->replications;
                printf("%.1lf\t", num);
            }

        }
        printf("\n");
    }


    /*for (int rep = 0; rep < replications; rep++) {// Display all replications
        printf("\n=== World %d ===\n", rep + 1);
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                printf(" %c ", simulation->world[rep].square[row][col].representation);
            }
            printf("\n");
        }
        printf("\n");
    }*/

    for (int rep = 0; rep < simulation->replications; rep++) {
        for (int row = 0; row < simulation->size; row++) {
            free(simulation->world[rep].square[row]);// row
        }
        free(simulation->world[rep].square);// column
        free(simulation->world[rep].walker);
    }
    free(simulation->world);
}


