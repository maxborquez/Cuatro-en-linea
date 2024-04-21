#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 2

void *handle_client(void *socket_desc) {
    int client_fd = *(int *)socket_desc;
    ssize_t valread;
    char buffer[1024] = {0};

    while (1) {
        valread = read(client_fd, buffer, 1024 - 1);
        if (valread > 0) {
            std::cout << "Mensaje recibido: " << buffer << std::endl;

            // Send response to the client
            const char *hello = "Mensaje recibido por el servidor";
            send(client_fd, hello, strlen(hello), 0);
        } else {
            break;
        }
    }

    // Closing the connected socket
    close(client_fd);
    free(socket_desc);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return -1;
    }

    int PORT = atoi(argv[1]);
    int server_fd, new_socket, *new_client_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Error creating listening socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "Error calling bind()" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        std::cout << "Error calling listen()" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "\nNuevo juego inciado, esperando jugadores\n";

    pthread_t thread_id[MAX_CLIENTS];
    int i = 0;

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        new_client_socket = (int *)malloc(1);
        *new_client_socket = new_socket;

        if (pthread_create(&thread_id[i++], NULL, handle_client, (void *)new_client_socket) < 0) {
            perror("could not create thread");
            return -1;
        }

        // Reset thread index when it reaches the maximum number of clients
        if (i >= MAX_CLIENTS) {
            i = 0;
            while (i < MAX_CLIENTS) {
                pthread_join(thread_id[i++], NULL);
            }
            i = 0;
        }
    }

    // Closing the listening socket
    close(server_fd);
    return 0;
}
