#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

char *runScript(void);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP_ADDR> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip_addr = argv[1];
    int port = atoi(argv[2]);

    int server_fd, client_fd;
    struct sockaddr_in sa;
    struct in_addr ia;
    int addrlen = sizeof(sa);
    char message[] = "Hello!\nPlease enter a command:\n(1) Check for parking spots\n(2) Close connection\n\nSelection: ";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    ia.s_addr = inet_addr(ip_addr);
    sa.sin_family = AF_INET;
    sa.sin_addr = ia;
    sa.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((client_fd = accept(server_fd, (struct sockaddr *)&sa, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int bytes_sent = send(client_fd, message, strlen(message), 0);
        if (bytes_sent > 0)
        {
            printf("Welcome message sent to client\n");
        }

        char buf[BUF_SIZE];
        memset(buf, 0, BUF_SIZE); /* clear the buffer */

        int bytes_recv = recv(client_fd, buf, BUF_SIZE, 0);
        if (bytes_recv > 0)
        {
            printf("Command received from client: %s\n", buf);
            sprintf(message, "%s\n", buf);
            send(client_fd, message, strlen(message), 0);
        }

        int command = atoi(buf);
        if (command == 1)
        {
            char *out = runScript();
            if (out != NULL)
            {
                printf("Script output: %s\n", out);
                sprintf(message, "Spots Available: %s\n", out);
                int bytes_sent = send(client_fd, message, strlen(message), 0);
                if (bytes_sent > 0)
                {
                    printf("Message sent to client\n");
                }
                else
                {
                    perror("send");
                }
                free(out); // Free allocated memory
            }
            else
            {
                perror("runScript");
            }
        }

        if (command == 2)
        {
            close(client_fd);
        }
    }
    return 0;
}

char *runScript()
{
    char *buffer = malloc(1024); // Dynamically allocate memory
    if (!buffer)
    {
        perror("malloc");
        return NULL;
    }

    FILE *pipe = popen("python3 ./ImageProcessing/main.py", "r");
    if (!pipe)
    {
        perror("popen");
        free(buffer); // Free allocated memory before returning NULL
        return NULL;
    }

    fgets(buffer, 1024 * 50, pipe); // Read into the allocated buffer

    pclose(pipe);

    return buffer;
}
