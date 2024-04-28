#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <cstdlib>

using namespace std;

// Función para manejar los mensajes que controlan el juego
void* jugar(void* args) {
    int socket_cliente = *((int*)args);  // Recuperar socket del cliente
    free(args);  // Liberar memoria reservada para el socket

    struct sockaddr_in direccionCliente;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(socket_cliente, (struct sockaddr*)&direccionCliente, &addr_size);  // Obtener dirección del cliente

    char buffer[1024];  // Buffer para almacenar los mensajes recibidos
    memset(buffer, '\0', sizeof(char)*1024);
    int n_bytes = 0;

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(direccionCliente.sin_addr), ip, INET_ADDRSTRLEN);  // Convertir IP a string

    cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Nuevo jugador." << endl;

    const char *turn_msg = "Jugador comienza.\n";
    send(socket_cliente, turn_msg, strlen(turn_msg), 0);  // Enviar mensaje inicial al client

    while ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0) {
        buffer[n_bytes] = '\0';  // Asegurar final del string

        if (buffer[0] == 'Q') {  // Salir del juego si recibe 'Q'
            cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Sale del juego." << endl;
            close(socket_cliente);
            return NULL;
        }

        // Colocar ficha en columna
        if (buffer[0] == 'c' && buffer[1] >= '1' && buffer[1] <= '7') {
            cout << "[" << ip << ":" << ntohs(direccionCliente.sin_port) << "] Columna: " << buffer[1] << endl;

            char server_move[4];
            sprintf(server_move, "c%d\n", (rand() % 7) + 1); // Generar movimiento aleatorio del servidor
            send(socket_cliente, server_move, strlen(server_move), 0);  // Responder con movimiento del servidor
        } else {
            send(socket_cliente, "error\n", 6, 0);  // Enviar mensaje de error si el comando es inválido.
        }
    }
    close(socket_cliente);  // Cerrar el socket al finalizar la comunicación.
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;  // Verificar que se haya proporcionado el puerto.
        return 1;
    }

    cout << "Calling listening ...\n" << std::endl;
    int port = atoi(argv[1]);  // Convertir el puerto a entero.
    int socket_server = socket(AF_INET, SOCK_STREAM, 0);  // Crear socket de servidor
    if (socket_server < 0) {
        cout << "Error creating listening socket\n" << endl;
        return 1;
    }

    struct sockaddr_in direccionServidor;
    memset(&direccionServidor, 0, sizeof(direccionServidor));  // Configurar dirección del servidor
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port = htons(port);

    if (bind(socket_server, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error calling bind()\n" << endl;  // Enlazar socket
        return 1;
    }

    if (listen(socket_server, 1024) < 0) {
        cout << "Error calling listen()\n" << endl;  // Poner el servidor a escuchar conexiones
        return 1;
    }

    cout << "Waiting client request ...\n" << std::endl;
    struct sockaddr_in direccionCliente;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    while (true) {
        int* socket_cliente = (int*)malloc(sizeof(int));  // Reservar memoria para socket del cliente
        *socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size);  // Aceptar conexión entrante
        if (*socket_cliente < 0) {
            cout << "Error calling accept()\n" << endl;
            free(socket_cliente);
            continue;
        }

        pthread_t thread_id;  // Crear hilos para permitir multiples conexiones
        if (pthread_create(&thread_id, NULL, jugar, (void*)socket_cliente) < 0) {
            cout << "Error creating thread\n" << endl;
            close(*socket_cliente);
            free(socket_cliente);
            continue;
        }
        pthread_detach(thread_id);  // Liberar recursos del hilo
    }

    return 0;
}
