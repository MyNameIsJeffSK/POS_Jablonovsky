#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include "Simulation.h"
#include "Socket.h"


#define MAX_CLIENTS 10
#define SERVER_PORT 25565

typedef struct {
    int size;
    int replications;
    int up_p;
    int down_p;
    int left_p;
    int right_p;
    int steps;
    int mode;
    int type; // 0=NonObstacle, 1=Obstacle
} SimulationRequest;


typedef struct {
    int server_socket;
    pthread_mutex_t clients_mutex;
    int active_clients;
    int is_running;
} ServerContext;


typedef struct {
    ServerContext* context;
    int client_socket;
    struct sockaddr_in client_addr;
} ClientThreadData;


ServerContext* server_init() {
    ServerContext* context = (ServerContext*)malloc(sizeof(ServerContext));
    if (!context) {
        perror("Failed to allocate server context");
        return NULL;
    }


    pthread_mutex_init(&context->clients_mutex, NULL);
    
    context->server_socket = -1;
    context->active_clients = 0;
    context->is_running = 0;

    return context;
}


void server_cleanup(ServerContext* context) {
    if (!context) return;


    if (context->is_running) {
        context->is_running = 0;
        if (context->server_socket != -1) {
            passive_socket_destroy(context->server_socket);
        }
    }

    pthread_mutex_destroy(&context->clients_mutex);
    free(context);
}


void* handle_client_thread(void* arg) {
    ClientThreadData* data = (ClientThreadData*)arg;
    ServerContext* context = data->context;
    int client_socket = data->client_socket;

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(data->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(data->client_addr.sin_port);

    free(data);

    printf("Client %s:%d connected\n", client_ip, client_port);


    const char* welcome = "Connected to Random Walk Simulation Server\n";
    send(client_socket, welcome, strlen(welcome), 0);


    while (context->is_running) {
        
        // Namiesto priameho prijatia štruktúry:
int values[9];
int bytes_received = recv(client_socket, values, sizeof(values), 0);

if (bytes_received != sizeof(values)) {
    printf("Invalid request size: %d (expected %lu)\n", bytes_received, sizeof(values));
    break;
}


SimulationRequest request;
request.size = ntohl(values[0]);
request.replications = ntohl(values[1]);
request.up_p = ntohl(values[2]);
request.down_p = ntohl(values[3]);
request.left_p = ntohl(values[4]);
request.right_p = ntohl(values[5]);
request.steps = ntohl(values[6]);
request.mode = ntohl(values[7]);
request.type = ntohl(values[8]);

printf("DEBUG received: %d %d %d %d %d %d %d %d %d\n",
       request.size, request.replications, request.up_p, request.down_p,
       request.left_p, request.right_p, request.steps, request.mode, request.type);
        printf("\n=== New Simulation Request ===\n");
        printf("Client: %s:%d\n", client_ip, client_port);
        printf("World size: %dx%d\n", request.size, request.size);
        printf("Replications: %d\n", request.replications);
        printf("Max steps: %d\n", request.steps);
        printf("Probabilities: Up=%d%%, Down=%d%%, Left=%d%%, Right=%d%%\n",
               request.up_p, request.down_p, request.left_p, request.right_p);
        printf("Mode: %s\n", request.mode == 1 ? "Interactive" : "Summary");
        printf("World type: %s\n", request.type == 1 ? "With obstacles" : "Without obstacles");
        printf("===============================\n");


        if (request.up_p + request.down_p + request.left_p + request.right_p != 100) {
            const char* error = "ERROR: Probabilities must sum to 100\n";
            send(client_socket, error, strlen(error), 0);
            printf("Validation failed: Probabilities don't sum to 100\n");
            continue;
        }

        if (request.size < 2 || request.size > 100) {
            const char* error = "ERROR: Invalid world size (must be 2-100)\n";
            send(client_socket, error, strlen(error), 0);
            printf("Validation failed: Invalid world size %d\n", request.size);
            continue;
        }

        if (request.replications <= 0) {
            const char* error = "ERROR: Replications must be positive\n";
            send(client_socket, error, strlen(error), 0);
            printf("Validation failed: Invalid replications %d\n", request.replications);
            continue;
        }


        const char* ack = "Simulation request accepted. Processing...\n";
        send(client_socket, ack, strlen(ack), 0);
        Simulation simulation;
        simulation.replications = request.replications;
        simulation.down_p = request.down_p;
        simulation.up_p = request.up_p;
        simulation.left_p = request.left_p;
        simulation.right_p = request.right_p;
        simulation.size = request.size;
        simulation.steps = request.steps;
        simulation.mode = request.mode;
        simulation.type = request.type;
        simulation.world = malloc(request.replications * sizeof(World));
        if (simulation.world == NULL) {
        const char* error = "ERROR: Memory allocation failed\n";
            send(client_socket, error, strlen(error), 0);
            printf("Memory allocation failed for simulation world\n");
            continue;
        }


        for (int i = 0; i < request.replications; i++) { // inicialize all worlds
            init_World(&simulation.world[i]);
        }
        
        get_info(&simulation);
        simulation.world = NULL;
        free(simulation.world);
        
        printf("Processing simulation for client %s:%d...\n", client_ip, client_port);

        const char* complete = "Simulation processing complete (placeholder)\n";
        send(client_socket, complete, strlen(complete), 0);
    }

    // Cleanup 
    pthread_mutex_lock(&context->clients_mutex);
    context->active_clients--;
    printf("Client %s:%d disconnected. Active clients: %d\n",
           client_ip, client_port, context->active_clients);
    pthread_mutex_unlock(&context->clients_mutex);

    close(client_socket);

    return NULL;
}


void server_run(ServerContext* context) {
    if (!context) return;

    context->server_socket = passive_socket_init(SERVER_PORT);
    if (context->server_socket < 0) {
        fprintf(stderr, "Failed to create server socket\n");
        return;
    }

    printf("=== Random Walk Simulation Server ===\n");
    printf("Listening on port %d\n", SERVER_PORT);
    printf("Maximum clients: %d\n", MAX_CLIENTS);
    printf("Waiting for connections...\n\n");

    context->is_running = 1;


    while (context->is_running) {

        int client_socket = passive_socket_wait_for_client(context->server_socket);
        if (client_socket < 0) {
            if (context->is_running) {
                perror("Failed to accept client");
            }
            continue;
        }


        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        getpeername(client_socket, (struct sockaddr*)&client_addr, &client_len);


        pthread_mutex_lock(&context->clients_mutex);
        if (context->active_clients >= MAX_CLIENTS) {
            printf("Max clients reached, rejecting connection\n");
            const char* msg = "SERVER_FULL: Maximum clients reached\n";
            send(client_socket, msg, strlen(msg), 0);
            close(client_socket);
            pthread_mutex_unlock(&context->clients_mutex);
            continue;
        }
        context->active_clients++;
        printf("Active clients: %d\n", context->active_clients);
        pthread_mutex_unlock(&context->clients_mutex);


        ClientThreadData* thread_data = (ClientThreadData*)malloc(sizeof(ClientThreadData));
        thread_data->context = context;
        thread_data->client_socket = client_socket;
        thread_data->client_addr = client_addr;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client_thread, thread_data) != 0) {
            perror("Failed to create client thread");

            pthread_mutex_lock(&context->clients_mutex);
            context->active_clients--;
            pthread_mutex_unlock(&context->clients_mutex);

            close(client_socket);
            free(thread_data);
            continue;
        }
        pthread_detach(thread_id);
       
    }
  free(context);
}


void handle_signal(int sig, ServerContext* context) {
    printf("\nReceived signal %d, shutting down server...\n", sig);
    if (context) {
        context->is_running = 0;
    }
}


int main(int argc, char* argv[]) {
    printf("Starting Random Walk Simulation Server...\n");

    // Initialize server context
    ServerContext* server = server_init();
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }

    struct sigaction sa;
    sa.sa_handler = (void (*)(int))handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGTERM, &sa, NULL);  // Termination request

    server_run(server);


    server_cleanup(server);

    printf("Server shutdown complete\n");
    return 0;
}
