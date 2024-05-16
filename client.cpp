#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <ctime> // Para srand y rand
#include <cstdlib>

using namespace std;

// Función para imprimir el tablero
void imprimirTablero(char tablero[6][7]) {
    cout << "TABLERO" << endl;
    for (int i = 5; i >= 0; --i) {
        for (int j = 0; j < 7; ++j) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
    cout << "-------------" << endl;
    cout << "1 2 3 4 5 6 7" << endl;
}

// Función para verificar si una columna está llena
bool columnaLlena(char tablero[6][7], int columna) {
    return tablero[5][columna] != ' ';
}

// Función para verificar si un jugador ha ganado
bool verificarGanador(char tablero[6][7], char jugador) {
    // Verificar horizontalmente
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila][col + 1] == jugador &&
                tablero[fila][col + 2] == jugador &&
                tablero[fila][col + 3] == jugador) {
                return true;
            }
        }
    }

    // Verificar verticalmente
    for (int col = 0; col < 7; ++col) {
        for (int fila = 0; fila < 3; ++fila) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col] == jugador &&
                tablero[fila + 2][col] == jugador &&
                tablero[fila + 3][col] == jugador) {
                return true;
            }
        }
    }

    // Verificar diagonalmente (ascendente)
    for (int fila = 3; fila < 6; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila - 1][col + 1] == jugador &&
                tablero[fila - 2][col + 2] == jugador &&
                tablero[fila - 3][col + 3] == jugador) {
                return true;
            }
        }
    }

    // Verificar diagonalmente (descendente)
    for (int fila = 0; fila < 3; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col + 1] == jugador &&
                tablero[fila + 2][col + 2] == jugador &&
                tablero[fila + 3][col + 3] == jugador) {
                return true;
            }
        }
    }

    return false;
}

// Función para verificar si hay empate
bool verificarEmpate(char tablero[6][7]) {
    for (int col = 0; col < 7; ++col) {
        if (tablero[5][col] == ' ') {
            return false; // Todavía hay espacio en la columna, no hay empate
        }
    }
    return true; // Todas las columnas están llenas y es empate
}

int main(int argc, char const *argv[]) {
    // Verificar que se haya proporcionado la dirección IP y el puerto
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <ip> <port>" << endl;
        return -1;
    }

    // Crear socket del cliente
    int port = atoi(argv[2]);
    int status, valread, socket_cliente;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char mensaje[1024];

    // Inicializar tablero
    char tablero[6][7];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            tablero[i][j] = ' ';
        }
    }

    // Crear socket del cliente
    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\n Socket creation error \n" << endl;
        return -1;
    }

    // Configurar dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convertir dirección IP a binario
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        cout << "\nInvalid address/ Address not supported \n" << endl;
        return -1;
    }
    // Conectar al servidor
    if ((status = connect(socket_cliente, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        cout << "\nConnection Failed \n" << endl;
        return -1;
    }

    cout << "Bienvenido al juego de 4 en linea" << endl; // Mensaje de bienvenida
    cout << "Escriba c1,c2,c3,c4,c5,c6 o c7 para poner una ficha en una columna" << endl;
    cout << "Escriba Q para salir del juego \n" << endl;

    // Determinar de manera aleatoria quién comienza el juego
    srand(time(0)); // Inicializar la semilla para rand
    bool servidorComienza = rand() % 2 == 0;

    if (servidorComienza) {
        cout << "Servidor comienza" << endl;
        send(socket_cliente, "start\n", 6, 0); // Enviar mensaje al servidor para que inicie el juego
        valread = read(socket_cliente, buffer, 1024 - 1);
        cout << "Movimiento del servidor: " << buffer << endl;

        int columna_servidor = buffer[1] - '1';
        for (int i = 0; i < 6; ++i) {
            if (tablero[i][columna_servidor] == ' ') {
                tablero[i][columna_servidor] = 'S';
                break;
            }
        }

        imprimirTablero(tablero);
    } else {
        cout << "Jugador comienza" << endl;
    }

    cout << "\n" << endl;

    // Juego
    while (1) {
        cout << "Escriba su movimiento: ";
        fgets(mensaje, 1024, stdin);

        // Si el mensaje es 'Q', desconectar al cliente
        if (mensaje[0] == 'Q') {
            cout << "Client disconnected" << endl;
            close(socket_cliente);
            break;
        }

        // Si es q minúscula, se le notifica al usuario
        if (mensaje[0] == 'q') {
            cout << "Entrada inválida, ¿tal vez queria escribir Q?" << endl;
            continue;
        }

        // Si el mensaje no es c1 a c7, se le notifica al usuario
        if (mensaje[1] < '1' || mensaje[1] > '7') {
            cout << "Entrada inválida. Eliga un número de columna entre 1 y 7." << endl;
            continue;
        }

        int columna_cliente = mensaje[1] - '1';

        // Verificar si la columna está llena
        if (columnaLlena(tablero, columna_cliente)) {
            cout << "Columna " << columna_cliente + 1 << " está llena. Eliga otra columna." << endl;
            continue;
        }

        // Colocar ficha en la columna seleccionada
        for (int i = 0; i < 6; ++i) {
            if (tablero[i][columna_cliente] == ' ') {
                tablero[i][columna_cliente] = 'C';
                break;
            }
        }

        // Imprimir el tablero después del movimiento del cliente
        imprimirTablero(tablero);

        // Enviar mensaje al servidor
        send(socket_cliente, mensaje, strlen(mensaje), 0);

        // Verificar si el cliente ha ganado
        if (verificarGanador(tablero, 'C')) {
            const char* mensaje = "\n¡Jugador ha ganado!";
            send(socket_cliente, mensaje, strlen(mensaje), 0); // Enviar mensaje al servidor
            cout << mensaje << endl;
            break;
        } else if (verificarEmpate(tablero)) {
            const char* mensaje = "\n¡El juego ha empatado!";
            send(socket_cliente, mensaje, strlen(mensaje), 0); // Enviar mensaje al servidor
            cout << mensaje << endl;
            break;
        }

        valread = read(socket_cliente, buffer, 1024 - 1);
        cout << "Movimiento del servidor: " << buffer << endl;

        int columna_servidor = buffer[1] - '1';
        for (int i = 0; i < 6; ++i) {
            if (tablero[i][columna_servidor] == ' ') {
                tablero[i][columna_servidor] = 'S';
                break;
            }
        }

        // Imprimir el tablero después del movimiento del servidor
        imprimirTablero(tablero);

        // Verificar si el servidor ha ganado
        if (verificarGanador(tablero, 'S')) {
            const char* mensaje = "\nEl servidor ha ganado!";
            send(socket_cliente, mensaje, strlen(mensaje), 0); // Enviar mensaje al servidor
            cout << mensaje << endl;
            break;
        } else if (verificarEmpate(tablero)) {
            const char* mensaje = "\n¡El juego ha empatado!";
            send(socket_cliente, mensaje, strlen(mensaje), 0); // Enviar mensaje al servidor
            cout << mensaje << endl;
            break;
        }

        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
