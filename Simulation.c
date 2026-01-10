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

void display_World_summary(World * world) {
    // Implementation for summary mode
}

void display_World_Interactive(World * world) {
    // Implementation for interactive mode
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
    if (start_x == 0 && start_y == 0) return;  // Finish square

    // Store starting position
    int original_x = world->walker->pos_x;
    int original_y = world->walker->pos_y;

    // Reset walker for this simulation
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

        // Calculate new position
        switch (dir) {
            case Up:    new_x--; break;
            case Down:  new_x++; break;
            case Left:  new_y--; break;
            case Right: new_y++; break;
        }

        // Handle wrap-around
        if (new_x < 0) new_x = world->size - 1;
        else if (new_x >= world->size) new_x = 0;

        if (new_y < 0) new_y = world->size - 1;
        else if (new_y >= world->size) new_y = 0;

        // Check if new position has obstacle
        if (world->square[new_x][new_y].representation == '#') {
            // Hit obstacle - just count the step
            world->walker->steps_done++;
        } else {
            // Move to new position
            world->walker->pos_x = new_x;
            world->walker->pos_y = new_y;
            world->walker->steps_done++;
        }

        // Check if reached finish
        if (world->walker->pos_x == 0 && world->walker->pos_y == 0) {
            break;
        }

        steps_taken++;
    }

    // Store result in the square
    world->square[start_x][start_y].value = world->walker->steps_done;

    // Restore original position (if needed)
    world->walker->pos_x = original_x;
    world->walker->pos_y = original_y;
}

void calculate_World_summary(World * world) {
    for (int i = 0; i < world->size; i++) {
        for (int j = 0; j < world->size; j++) {
            world->square[i][j].value = 0;
            if ((i == 0 && j == 0) || world->square[i][j].representation == '#') {
                continue;
            }
            calculate_square(world, i, j);
        }
    }
}

void get_info(Simulation * simulation) {
    int size;
    int replications;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    int steps;
    int mode;
    WorldType type;

    srand(time(NULL));


    while (1) {
        printf("Insert map size\n");
        scanf("%d", &size);
        if (size >= 2) {
            break;
        }
        printf("Wrong map size!\n");
    }// size


    while (1) {
        printf("Insert number of replications\n");
        scanf("%d", &replications);
        if (replications > 0) {
            break;
        }
        printf("Wrong number of replications!\n");
    }// replications

    simulation->world = malloc(replications * sizeof(World));
    if (simulation->world == NULL) {
        printf("Memory allocation failed for world array!\n");
        return;
    }


    while (1) {
        printf("Insert probabilities up, down, left, right (Must equal to 100)\n");
        scanf("%d %d %d %d", &up_p, &down_p, &left_p, &right_p);
        if ((up_p + down_p + left_p + right_p) == 100) {
            break;
        }
        printf("Wrong probabilities, try again!\n");
    }// probabilities


    while (1) {
        printf("Insert number of steps\n");
        scanf("%d", &steps);
        if (steps >= 0) {
            break;
        }
        printf("Wrong amount of steps\n");
    }// steps

    int choice;
    while (1) {
        printf("1. Interactive mode 2. Summary mode\n");
        scanf("%d", &choice);
        if (choice == 1) {
            mode = 1;
            break;
        }
        if (choice == 2) {
            mode = 2;
            break;
        }
        printf("Wrong mode!\n");
    }// mode

    choice = 0;
    while (1) {
        printf("Select world type:\n 1. With obstacles\n 2. Without obstacles\n");
        scanf("%d", &choice);
        if (choice == 1) {
            type = Obstacle;
            break;
        }
        if (choice == 2) {
            type = NonObstacle;
            break;
        }
        printf("Wrong type!\n");
    }// world type


    for (int i = 0; i < replications; i++) {

        simulation->world[i].replications = replications;
        simulation->world[i].size = size;
        simulation->world[i].type = type;
        simulation->world[i].mode = mode;

        simulation->world[i].walker = malloc(sizeof(Walker));
        if (simulation->world[i].walker == NULL) {
            printf("Memory allocation failed for walker!\n");
            return;
        }
        simulation->world[i].walker->steps = steps;
        simulation->world[i].walker->steps_done = 0;
        simulation->world[i].walker->up_p = up_p;
        simulation->world[i].walker->down_p = down_p;
        simulation->world[i].walker->left_p = left_p;
        simulation->world[i].walker->right_p = right_p;
        simulation->world[i].walker->pos_x = 0;
        simulation->world[i].walker->pos_y = 0;
        simulation->world[i].walker->world = &simulation->world[i];

        simulation->world[i].square = malloc(size * sizeof(Square *)); // collumn
        if (simulation->world[i].square == NULL) {
            printf("Memory allocation failed for square rows!\n");
            return;
        }

        for (int row = 0; row < size; row++) { // row
            simulation->world[i].square[row] = malloc(size * sizeof(Square));
            if (simulation->world[i].square[row] == NULL) {
                printf("Memory allocation failed for square columns!\n");
                return;
            }
        }

        if (i == 0) {// generate first world
            for (int row = 0; row < size; row++) {
                for (int col = 0; col < size; col++) {
                    if (row == 0 && col == 0) {
                        set_finish(&simulation->world[i].square[row][col]);
                    } else {
                        initSquare(&simulation->world[i].square[row][col],
                                  type == Obstacle);
                    }
                }
            }
        } else {// copy grid to all the other worlds
            for (int row = 0; row < size; row++) {
                for (int col = 0; col < size; col++) {
                    simulation->world[i].square[row][col].representation =
                        simulation->world[0].square[row][col].representation;
                }
            }
        }
        calculate_World_summary(&simulation->world[i]);
    }




    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
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
    printf("Results\n");
    double result = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            for (int rep = 0; rep < replications; rep++) {
                result += simulation->world[rep].square[row][col].value;
            }
            if ((row == 0 && col == 0 )|| simulation->world[0].square[row][col].representation == '#') {
                printf("%c\t", simulation->world[0].square[row][col].representation);
            } else {
                result = (double)result / (double)replications;
                printf("%.1lf\t", result);
                result = 0;
            }
        }
        printf("\n");
    }


    /*for (int rep = 0; rep < replications; rep++) {// Display
        printf("\n=== World %d ===\n", rep + 1);
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                printf(" %c ", simulation->world[rep].square[row][col].representation);
            }
            printf("\n");
        }
        printf("\n");
    }*/

    for (int rep = 0; rep < replications; rep++) {
        for (int row = 0; row < size; row++) {
            free(simulation->world[rep].square[row]);// row
        }
        free(simulation->world[rep].square);// collumn
        free(simulation->world[rep].walker);
    }
    free(simulation->world);
}

int main() {
    srand(time(NULL));
    Simulation simulation;
    get_info(&simulation);
    return 0;
}