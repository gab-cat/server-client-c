#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define BOARD_SIZE 5

void die_with_error(char *error_msg) {
    printf("%s", error_msg);
    exit(-1);
}

void initialize_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '~';
        }
    }
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int sock, port_no, n;
    struct sockaddr_in addr;
    struct hostent *server;
    int active = 1;
    char buffer[256];
    char board[BOARD_SIZE][BOARD_SIZE];

    printf("Choose mode (1 for server, 2 for client): ");
    int mode;
    scanf("%d", &mode);

    if (mode == 1) {
        // Server mode
        int server_sock, client_sock, client_size;
        struct sockaddr_in server_addr, client_addr;

        if (argc < 2) {
            printf("Usage: %s port_no\n", argv[0]);
            exit(1);
        }

        server_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sock < 0)
            die_with_error("Error: socket() Failed.");

        bzero((char *)&server_addr, sizeof(server_addr));
        port_no = atoi(argv[1]);
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port_no);

        if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            die_with_error("Error: bind() Failed.");

        listen(server_sock, 5);
        printf("SERVER STARTED: Waiting for connection(s) ...\n\n");

        client_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);
        if (client_sock < 0)
            die_with_error("Error: accept() Failed.");
        printf("Connection established!\n\n");

        sock = client_sock;
    } else if (mode == 2) {
        // Client mode
        if (argc < 3) {
            printf("Usage: %s hostname port_no\n", argv[0]);
            exit(1);
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
            die_with_error("Error: socket() Failed.");

        server = gethostbyname(argv[1]);
        if (server == NULL) {
            die_with_error("Error: No such host.");
        }

        port_no = atoi(argv[2]);
        bzero((char *)&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&addr.sin_addr.s_addr, server->h_length);
        addr.sin_port = htons(port_no);

        printf("CLIENT STARTED: Looking for host '%s' at port %s \n", argv[1], argv[2]);

        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            die_with_error("Error: connect() Failed.");

        printf("\nConnected to server!\n\n");
    } else {
        printf("Invalid mode selected.\n");
        exit(1);
    }

    initialize_board(board);
    print_board(board);

    while (active) {
        bzero(buffer, 256);
        n = recv(sock, buffer, 255, 0);
        if (n < 0)
            die_with_error("Error: recv() Failed.");
        printf("[opponent] > %s", buffer);

        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
            active = 0;
            break;
        }
        printf("< ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = send(sock, buffer, strlen(buffer), 0);
        if (n < 0)
            die_with_error("Error: send() Failed.");
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
            active = 0;
            break;
        }
    }

    printf("Connection closed!\n");
    close(sock);

    return 0;
}
