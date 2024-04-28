/*
 * server.cpp
 *
 * g++ -std=c++11 -pthread server.cpp -o server
 *
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <cstdlib>

using namespace std;

void* jugar(void* args) {
    int socket_cliente = *((int*)args);
    struct sockaddr_in direccionCliente;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(socket_cliente, (struct sockaddr*)&direccionCliente, &addr_size);

    char buffer[1024];
    memset(buffer, '\0', sizeof(char)*1024);
    int n_bytes = 0;

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(direccionCliente.sin_addr), ip, INET_ADDRSTRLEN);

    cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Nuevo jugador." << endl;

    const char *turn_msg = "Jugador comienza.\n";
    send(socket_cliente, turn_msg, strlen(turn_msg), 0);

    while ((n_bytes = recv(socket_cliente, buffer, 1024, 0))) {
        buffer[n_bytes] = '\0';

        if (buffer[0] == 'Q') {
            cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Sale del juego." << endl;
            close(socket_cliente);
            break;
        }

        if (buffer[0] == 'c' && buffer[1] >= '1' && buffer[1] <= '7') {
            cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Columna: " << buffer[1] << endl;

            // Aquí se selecciona aleatoriamente una columna para el servidor
            char server_move[4];
            sprintf(server_move, "c%d\n", (rand() % 7) + 1);
            send(socket_cliente, server_move, strlen(server_move), 0);
        } else {
            send(socket_cliente, "error\n", 6, 0);
        }
    }
    close(socket_cliente);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        return 1;
    }

    int port = atoi(argv[1]);
    int socket_server = 0;
    struct sockaddr_in direccionServidor, direccionCliente;

    cout << "Creating listening socket ...\n";
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating listening socket\n";
        exit(EXIT_FAILURE);
    }

    cout << "Configuring socket address structure ...\n";
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family      = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port        = htons(port);

    cout << "Binding socket ...\n";
    if (bind(socket_server, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error calling bind()\n";
        exit(EXIT_FAILURE);
    }

    cout << "Calling listening ...\n";
    if (listen(socket_server, 1024) < 0) {
        cout << "Error calling listen()\n";
        exit(EXIT_FAILURE);
    }

    cout << "Waiting client request ...\n";
    while (true) {
        int socket_cliente;
        pthread_t thread_id;
        socklen_t addr_size = sizeof(struct sockaddr_in);

        if ((socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size)) < 0) {
            cout << "Error calling accept()\n";
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&thread_id, NULL, jugar, (void*)&socket_cliente) < 0) {
            cout << "Error creating thread\n";
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
