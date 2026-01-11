#ifndef SEMESTRALKA_SIMULATION_H
#define SEMESTRALKA_SIMULATION_H
#include "World.h"


typedef struct {
    World * world;
    int size;
    int replications;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    int steps;
    int mode;
    WorldType type;
}Simulation;


void calculate_World_summary(World * world);
void init_World(World * world);
void get_info(Simulation * simulation);



#endif //SEMESTRALKA_SIMULATION_H
