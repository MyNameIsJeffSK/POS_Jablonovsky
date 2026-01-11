#ifndef CLIENT_H
#define CLIENT_H

#include "Network.h"
#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Simulation.h"



#pragma pack(push, 1)  // Presné zarovnanie na 1 bajt
typedef struct {
    int size;
    int replications;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    int steps;
    int mode;
    int type;  // Používajte int namiesto WorldType
} client_t;
#pragma pack(pop)

int send_simulation_config(int socket, client_t * config);
void display_characteristics(World * world);
void display_results(World * world);

#endif // CLIENT_H
