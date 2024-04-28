#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <ctype.h>

// Función para imprimir el tablero
void printBoard(char board[6][7]) {
    std::cout << "TABLERO" << std::endl;
    for (int i = 5; i >= 0; --i) {
        for (int j = 0; j < 7; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------" << std::endl;
    std::cout << "1 2 3 4 5 6 7" << std::endl;
}

// Función para verificar si una columna está llena
bool isColumnFull(char board[6][7], int column) {
    return board[5][column] != ' ';
}

// Función para verificar si un jugador ha ganado
bool checkWin(char board[6][7], char player) {
    // Verificar horizontalmente
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == player &&
                board[row][col + 1] == player &&
                board[row][col + 2] == player &&
                board[row][col + 3] == player) {
                return true;
            }
        }
    }

    // Verificar verticalmente
    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            if (board[row][col] == player &&
                board[row + 1][col] == player &&
                board[row + 2][col] == player &&
                board[row + 3][col] == player) {
                return true;
            }
        }
    }

    // Verificar diagonalmente (ascendente)
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == player &&
                board[row - 1][col + 1] == player &&
                board[row - 2][col + 2] == player &&
                board[row - 3][col + 3] == player) {
                return true;
            }
        }
    }

    // Verificar diagonalmente (descendente)
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == player &&
                board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player &&
                board[row + 3][col + 3] == player) {
                return true;
            }
        }
    }

    return false;
}

// Función para verificar si hay empate
bool checkDraw(char board[6][7]) {
    for (int col = 0; col < 7; ++col) {
        if (board[5][col] == ' ') {
            return false; // Todavía hay espacio en la columna, no hay empate
        }
    }
    return true; // Todas las columnas están llenas y es empate
}

int main(int argc, char const *argv[]) {

    // Verificar que se haya proporcionado la dirección IP y el puerto
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    // Crear socket del cliente
    int PORT = atoi(argv[2]);
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024];

    // Inicializar tablero
    char board[6][7];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            board[i][j] = ' ';
        }
    }

    // Crear socket del cliente
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Configurar dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir dirección IP a binario
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // Conectar al servidor
    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }


    std::cout<< "Bienvenido al juego de 4 en linea" << std::endl; // Mensaje de bienvenida
    std::cout<< "escribe c1,c2,c3,c4,c5,c6 o c7 para poner una ficha en una columna" << std::endl;
    std::cout<< "escribe Q para salir del juego \n" << std::endl;

    // Leer mensaje de bienvenida del servidor
    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s", buffer);
    memset(buffer, 0, sizeof(buffer));

    std::cout<< "\n" << std::endl;

    // Juego
    while(1) {
        printf("Enter a message: ");
        fgets(message, 1024, stdin);

        // Si el mensaje es 'Q', desconectar al cliente
        if (message[0] == 'Q') { 
            printf("Client disconnected\n");
            close(client_fd);
            break;
        }

        // si es q minuscual, se le notifica al usuario
        if (message[0] == 'q') {
            printf("Entrada inválida, ¿tal vez querias escribir Q?\n");
            continue;
        }

        // si el mensaje no es c1 a c7, se le notifica al usuario
        if (message[1] < '1' || message[1] > '7') {
            printf("Entrada inválida. Elige un número de columna entre 1 y 7.\n");
            continue;
        }

        int column_client = message[1] - '1';

        // Verificar si la columna está llena
        if (isColumnFull(board, column_client)) {
            printf("Columna %d está llena. Elige otra columna.\n", column_client + 1);
            continue;
        }

        // Colocar ficha en la columna seleccionada
        for (int i = 0; i < 6; ++i) {
            if (board[i][column_client] == ' ') {
                board[i][column_client] = 'C';
                break;
            }
        }

        // Enviar mensaje al servidor
        send(client_fd, message, strlen(message), 0);

        // Verificar si el cliente ha ganado
        if (checkWin(board, 'C')) {
            printBoard(board);
            printf("\n¡Has ganado!\n");
            break;
        } else if (checkDraw(board)) {
            printBoard(board);
            printf("\n¡El juego está empatado!\n");
            break;
        }

        valread = read(client_fd, buffer, 1024 - 1);

        int column_server = buffer[1] - '1';
        for (int i = 0; i < 6; ++i) {
            if (board[i][column_server] == ' ') {
                board[i][column_server] = 'S';
                break;
            }
        }

        // Verificar si el servidor ha ganado
        if (checkWin(board, 'S')) {
            printBoard(board);
            printf("\nEl servidor ha ganado!\n");
            break;
        } else if (checkDraw(board)) {
            printBoard(board);
            printf("\n¡El juego está empatado!\n");
            break;
        }

        // Imprimir tablero y respuesta del servidor
        printf("Server response: %s\n", buffer);
        printBoard(board);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
