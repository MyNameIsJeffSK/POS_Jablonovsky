
#include "Client.h"
#include <pthread.h>

static pthread_mutex_t output_mutex;

void display_characteristics(World * world) {
    for (int row = 0; row < world->size; row++) {// display normal grid
        for (int col = 0; col < world->size; col++) {
            printf(" %c ", world->square[row][col].representation);
        }
        printf("\n");
    }
    printf("\n");
}

void display_results(World * world) {
    printf("======Results=====\n");
    printf("Average steps:\n");
    double result = 0;
    for (int row = 0; row < world->size; row++) {// display average steps
        for (int col = 0; col < world->size; col++) {
            for (int rep = 0; rep < world->replications; rep++) {
                result += world[rep].square[row][col].value;
            }
            if ((row == 0 && col == 0 )|| world->square[row][col].representation == '#') {
                printf("%c\t", world[0].square[row][col].representation);
            } else {
                result = (double)result / (double)world->replications;
                printf("%.1lf\t", result);
                result = 0;
            }
        }
        printf("\n");
    }

    printf("Probability of reaching X:\n");
    for (int row = 0; row < world->size; row++) {// display probability
        for (int col = 0; col < world->size; col++) {
            double num = 0;
            for (int rep = 0; rep < world->replications; rep++) {
                if (world[rep].square[row][col].successful_attempts == 1) {
                    num++;
                }
            }
            if ((row == 0 && col == 0 )|| world->square[row][col].representation == '#') {
                printf("%c\t", world->square[row][col].representation);
            } else {
                num = ((double)num * 100) / (double)world->replications;
                printf("%.1lf\t", num);
            }

        }
        printf("\n");
    }
}

void* receiveThreadProc(void* arg) {
    int socket = *(int*)arg;
    char buffer[1024];
    
    printf("Listening for server messages...\n");
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            printf("Connection to server lost or closed\n");
            break;
        }
        
        buffer[bytes_received] = '\0';
        printf("Server: %s", buffer);
    }
    
    return NULL;
}


int send_simulation_config(int socket, client_t * config) {
    // send info 1 by 1
    int values[9];
    values[0] = htonl(config->size);     
    values[1] = htonl(config->replications);
    values[2] = htonl(config->up_p);
    values[3] = htonl(config->down_p);
    values[4] = htonl(config->left_p);
    values[5] = htonl(config->right_p);
    values[6] = htonl(config->steps);
    values[7] = htonl(config->mode);
    values[8] = htonl(config->type);
    
    printf("DEBUG sending: %d %d %d %d %d %d %d %d %d\n",
           config->size, config->replications, config->up_p, config->down_p,
           config->left_p, config->right_p, config->steps, config->mode, config->type);
    
    if (send(socket, values, sizeof(values), 0) < 0) {
        perror("Failed to send config data");
        return -1;
    }
    
    return 0;
}
int main(int argc, char * argv[]) {
    char* server_address = "localhost";
    int server_port = 25565;
    int end = 0;
    int sd = -1;
    pthread_t receiveThread = 0;
    
    pthread_mutex_init(&output_mutex, NULL);
    
    if (argc > 1) {
        server_address = argv[1];
    }
    if (argc > 2) {
        server_port = atoi(argv[2]);
    }

    while (!end) {
        pthread_mutex_lock(&output_mutex);
        printf("\n=== Main Menu ===\n");
        printf("1. New simulation\n");
        printf("2. Join simulation\n");
        printf("3. Replay simulation\n");
        printf("4. Exit\n");
        printf("Choice: ");
        pthread_mutex_unlock(&output_mutex);
        
        int choice;
        scanf("%d", &choice);
        
        while(getchar() != '\n');

        switch (choice) {
            case 1: {
                // chceck connection
                if (sd < 0) {
                    pthread_mutex_lock(&output_mutex);
                    printf("Connecting to server %s:%d...\n", server_address, server_port);
                    pthread_mutex_unlock(&output_mutex);
                    
                    sd = connect_to_server(server_address, server_port);
                    if (sd < 0) {
                        pthread_mutex_lock(&output_mutex);
                        fprintf(stderr, "Error connecting to server\n");
                        pthread_mutex_unlock(&output_mutex);
                        break;
                    }
                    
                    if (pthread_create(&receiveThread, NULL, receiveThreadProc, &sd) != 0) {
                        pthread_mutex_lock(&output_mutex);
                        perror("Failed to create receive thread");
                        pthread_mutex_unlock(&output_mutex);
                        active_socket_destroy(sd);
                        sd = -1;
                        break;
                    }
                    
                    sleep(1);
                }
                
                client_t config;
                
                while (1) { // get info from user
                    pthread_mutex_lock(&output_mutex);
                    printf("Insert map size: ");
                    pthread_mutex_unlock(&output_mutex);
                    scanf("%d", &config.size);
                    if (config.size >= 2 && config.size <= MAX_SIZE) break;
                    pthread_mutex_lock(&output_mutex);
                    printf("Wrong map size! (2-%d)\n", MAX_SIZE);
                    pthread_mutex_unlock(&output_mutex);
                }
                
                while (1) {
                    pthread_mutex_lock(&output_mutex);
                    printf("Insert number of replications: ");
                    pthread_mutex_unlock(&output_mutex);
                    scanf("%d", &config.replications);
                    if (config.replications > 0) break;
                    pthread_mutex_lock(&output_mutex);
                    printf("Wrong number of replications!\n");
                    pthread_mutex_unlock(&output_mutex);
                }
                
                while (1) {
                    pthread_mutex_lock(&output_mutex);
                    printf("Insert probabilities up, down, left, right (Must equal to 100): ");
                    pthread_mutex_unlock(&output_mutex);
                    scanf("%d %d %d %d", &config.up_p, &config.down_p, 
                                         &config.left_p, &config.right_p);
                    int sum = config.up_p + config.down_p + config.left_p + config.right_p;
                    if (sum == 100) break;
                    pthread_mutex_lock(&output_mutex);
                    printf("Wrong probabilities! Sum = %d, must be 100\n", sum);
                    pthread_mutex_unlock(&output_mutex);
                }
                
                while (1) {
                    pthread_mutex_lock(&output_mutex);
                    printf("Insert maximum number of steps: ");
                    pthread_mutex_unlock(&output_mutex);
                    scanf("%d", &config.steps);
                    if (config.steps >= 0) break;
                    pthread_mutex_lock(&output_mutex);
                    printf("Wrong amount of steps!\n");
                    pthread_mutex_unlock(&output_mutex);
                }
                
                while (1) {
                    pthread_mutex_lock(&output_mutex);
                    printf("1. Interactive mode 2. Summary mode: ");
                    pthread_mutex_unlock(&output_mutex);
                    scanf("%d", &choice);
                    if (choice == 1) { config.mode = 1; break; }
                    if (choice == 2) { config.mode = 2; break; }
                    pthread_mutex_lock(&output_mutex);
                    printf("Wrong mode!\n");
                    pthread_mutex_unlock(&output_mutex);
                }
                
                while (1) {
                    pthread_mutex_lock(&output_mutex);
                    printf("Select world type:\n1. With obstacles\n2. Without obstacles\nChoice: ");
                    pthread_mutex_unlock(&output_mutex);
                    scanf("%d", &choice);
                    if (choice == 1) { config.type = Obstacle; break; }
                    if (choice == 2) { config.type = NonObstacle; break; }
                    pthread_mutex_lock(&output_mutex);
                    printf("Wrong type!\n");
                    pthread_mutex_unlock(&output_mutex);
                }
                
                pthread_mutex_lock(&output_mutex);
                printf("Sending configuration to server...\n");
                pthread_mutex_unlock(&output_mutex);
                
                if (send_simulation_config(sd, &config) < 0) {
                    pthread_mutex_lock(&output_mutex);
                    fprintf(stderr, "Failed to send configuration to server\n");
                    pthread_mutex_unlock(&output_mutex);
                } else {
                    pthread_mutex_lock(&output_mutex);
                    printf("Configuration sent successfully.\n");
                    pthread_mutex_unlock(&output_mutex);
                }
                break;
            }
            case 4:
                end = 1;
                break;
            default:
                pthread_mutex_lock(&output_mutex);
                printf("Option %d not implemented yet.\n", choice);
                pthread_mutex_unlock(&output_mutex);
                break;
        }
    }
    
    // Cleanup
    if (sd >= 0) {
        active_socket_destroy(sd);
    }
    
    if (receiveThread) {
        pthread_cancel(receiveThread);
        pthread_join(receiveThread, NULL);
    }
    
    pthread_mutex_destroy(&output_mutex);
    
    return 0;
}
