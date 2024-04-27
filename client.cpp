#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

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

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    int PORT = atoi(argv[2]);
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024];


    char board[6][7];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            board[i][j] = ' ';
        }
    }

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s", buffer);
    memset(buffer, 0, sizeof(buffer));

    while(1) {
        printf("Enter a message: ");
        fgets(message, 1024, stdin);

        int column_client = message[1] - '1';
        for (int i = 0; i < 6; ++i) {
            if (board[i][column_client] == ' ') {
                board[i][column_client] = 'C';
                break;
            }
        }

        send(client_fd, message, strlen(message), 0);

        if (message[0] == 'Q') {
            printf("Client disconnected\n");
            close(client_fd);
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

        printf("Server response: %s\n", buffer);
        printBoard(board);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
