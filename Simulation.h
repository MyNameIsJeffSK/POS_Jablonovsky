//
// Created by jakub on 07/01/2026.
//

#ifndef SEMESTRALKA_SIMULATION_H
#define SEMESTRALKA_SIMULATION_H
#include "World.h"


typedef struct {
    World * world;
}Simulation;

void display_World_summary(World * world);
void display_World_Interactive(World * world);
void calculate_World_summary(World * world);
void init_World(World * world);
void get_info(Simulation * simulation);



#endif //SEMESTRALKA_SIMULATION_H