#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>

#define SERVER_PORT 8080
#define MAX_CLIENTS 10
#define MAX_CLIENT_SOCKETS 10
#define MAX_DATA_SIZE 1024

typedef enum {
    MSG_CONNECT = 1,
    MSG_DISCONNECT,
    MSG_WORLD_CONFIG,
    MSG_SIMULATION_START,
    MSG_REPLICATION_UPDATE,
    MSG_RESULTS_DATA,
    MSG_MODE_CHANGE,
    MSG_DISPLAY_CHANGE,
    MSG_USER_ACTION,
    MSG_SIMULATION_END
} MessageType;

typedef enum {
    DISPLAY_STEPS = 1,        // average steps
    DISPLAY_PROBABILITY       // probability
} DisplayType;

#pragma pack(push, 1)
typedef struct {
    MessageType type;
    uint32_t client_id;
    uint32_t data_size;
} MessageHeader;

typedef struct {
    int world_size;
    int total_replications;
    int max_steps;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    int has_obstacles;
    char result_filename[256];
} WorldConfig;

typedef struct {
    int current_replication;
    int total_replications;
    int simulation_mode;      // 1 = interactive, 2 = summary
    int display_type;         // 1 = steps, 2 = probability
} SimulationState;

typedef struct {
    int row;
    int col;
    double average_steps;
    double probability;
    char representation;
} SquareResult;

typedef struct {
    int world_size;
    int total_squares;
    SquareResult results[MAX_DATA_SIZE / sizeof(SquareResult)];
} ResultsPacket;
#pragma pack(pop)

#endif // NETWORK_H
