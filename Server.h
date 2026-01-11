#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include "Simulation.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 25565


typedef struct {
    int server_socket;
    pthread_mutex_t clients_mutex;
    int active_clients;
    int is_running;
} ServerContext;

typedef struct {
    ServerContext* context;
    int client_socket;
    struct sockaddr_in *client_addr;
} ClientThreadData;


typedef struct {
    int size;
    int replications;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    int steps;
    int mode;
    WorldType type; //(0=NonObstacle, 1=Obstacle)
} SimulationRequest;


ServerContext* server_init();
void server_start(ServerContext* context);
void server_stop(ServerContext* context);
void server_cleanup(ServerContext* context);

#endif // SERVER_H
