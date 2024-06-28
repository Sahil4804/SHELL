#include "headers.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
// #include <storeboxa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PATH_SIZE 1024
#define port 80
void remove_html_tags(char *input)
{
    char *output = input; // Output pointer starts at the beginning of the input string
    int inside_tag = 0;   // Flag to track if we are inside an HTML tag

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] == '<')
        {
            inside_tag = 1; // We are inside an HTML tag
        }
        else if (input[i] == '>')
        {
            inside_tag = 0; // We are outside an HTML tag
        }
        else if (!inside_tag)
        {
            *output = input[i]; // Copy the character to the output
            output++;
        }
    }

    *output = '\0'; // Null-terminate the output string
}
// // void iman(char *input)
// {
//     char copy1[1024], copy2[1024];
//     strcpy(copy1, input);
//     strcpy(copy2, input);
//     char *input[1000];
//     int num_input = 0;
//     input[0] = strtok(copy2, " ");
//     while (input[num_input] != NULL)
//     {
//         input[++num_input] = strtok(NULL, " ");
//     }
//     char *input = input[1];
//     struct hostent *server;
//     struct sockaddr_in server_addr;
//     int sockfd, bytes_received;
//     char request[5000], response[5000];
//     server = gethostbyname("man.he.net");
//     if (server == NULL)
//     {
//         herror("Error in DNS resolution");
//         exit(1);
//     }
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0)
//     {
//         herror("herror opening socket");
//         exit(1);
//     }
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(80);
//     struct in_addr **addr_list = (struct in_addr **)server->h_addr_list;
//     if (addr_list[0] == NULL)
//     {
//         herror("herror in DNS resolution");
//         exit(1);
//     }
//     server_addr.sin_addr = *addr_list[0];

//     // Connect to the server
//     if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
//     {
//         herror("herror connecting to server");
//         close(sockfd);
//         exit(1);
//     }
//     char *man_page_start, *man_page_end, *section_start;

//     snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", input);
//     if (write(sockfd, request, strlen(request)) < 0)
//     {
//         herror("herror sending request");
//         close(sockfd);
//         exit(1);
//     }
//     while (bytes_received = read(sockfd, response, sizeof(response)) > 0)
//     {
//         // printf("%s\n", response);

//         // printf("%s\n", response);
//         char *token;
//         char *token_array[1000000];
//         for (int i = 0; i < 1000000; i++)
//         {
//             token_array[i] = (char *)malloc(sizeof(char) * 1024);
//         }

//         int token_count = 0;
//         char *delimiters = "\n"; // tokenizing the input
//         token = strtok(response, delimiters);
//         while (token != NULL)
//         {
//             token_array[token_count] = token;
//             token_count++;
//             token = strtok(NULL, delimiters);
//         }
//         int index1 = 0;
//         int index2 = 0;
//         for (int i = 0; i < token_count; i++)
//         {
//             if (strcmp(token_array[i], "NAME") == 0)
//             {
//                 index1 = i;
//                 break;
//             }
//         }
//         for (int i = 0; i < token_count; i++)
//         {
//             if (strcmp(token_array[i], "AUTHOR") == 0)
//             {
//                 index2 = i;
//                 break;
//             }
//         }
//         for (int i = index1; i < index2; i++)
//         {
//             printf("%s\n", token_array[i]);
//         }

//         if (bytes_received < 0)
//         {
//             herror("Error reading response");
//             close(sockfd);
//             exit(1);
//         }
//     }
//     // Close the socket
//     close(sockfd);
// }

// #include "headers.h"

void iman(char *input)
{
    struct addrinfo hh;
    struct addrinfo *result, *storebox;
    int sockfd;
    char *temp = (char *)malloc(sizeof(char) * 1024);
    char *t = (char *)malloc(sizeof(char) * 1024);
    strcpy(temp, input);
    // printf("%s\n", temp);
    int num = 0;
    char *token = (char *)malloc(sizeof(char) * 1024);
    token = strtok(temp, " ");

    // printf("%s\n", token);
    while (token != NULL)
    {
        // printf("%s\n", token);
        token = strtok(NULL, " ");
    }
    t = input;
    t = strtok(input, " \n\t");
    t = strtok(NULL, " \n\t");

    char *hostname = "man.he.net";
    char *path = (char *)malloc(sizeof(char) * PATH_SIZE);
    snprintf(path, PATH_SIZE, "/?topic=%s&section=all", t);
    // int port = 80; // HTTP port

    memset(&hh, 0, sizeof(struct addrinfo));
    hh.ai_family = AF_UNSPEC;     // Allow both IPv4 and IPv6
    hh.ai_socktype = SOCK_STREAM; // Use TCP

    int status = getaddrinfo("man.he.net", "http", &hh, &result);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    for (storebox = result; storebox != NULL; storebox = storebox->ai_next)
    {
        sockfd = socket(storebox->ai_family, storebox->ai_socktype, storebox->ai_protocol);
        if (sockfd == -1)
        {
            perror("socket");
            continue;
        }

        if (connect(sockfd, storebox->ai_addr, storebox->ai_addrlen) == -1)
        {
            perror("connect");
            close(sockfd); // Close the socket
            continue;
        }

        printf("Connected to server\n");
        break;
    }

    if (storebox == NULL)
    {
        fprintf(stderr, "Failed to connect to any IP address\n");
        exit(EXIT_FAILURE);
    }

    char request[1024];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n\r\n",
             path, hostname);

    ssize_t bytes_sent = send(sockfd, request, strlen(request), 0);
    if (bytes_sent == -1)
    {
        perror("send");
        close(sockfd);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    printf("HTTP request sent\n");
    char response[100000];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0)
    {
        response[bytes_received] = '\0';
        remove_html_tags(response);
        // printf("Received %ld bytes:\n%s", bytes_received, response); // Debugging: Print the received data
        int f = 0;
        for (int i = 0; i < strlen(response); i++)
        {
            char *name = "NAME";
            char *author = "AUTHOR";
            if (response[i] == 'N' && response[i + 1] == 'A' && response[i + 2] == 'M' && response[i + 3] == 'E')
            {
                f = 1;
            }
            if (response[i] == 'A' && response[i + 1] == 'U' && response[i + 2] == 'T' && response[i + 3] == 'H' && response[i + 4] == 'O' && response[i + 5] == 'R')
            {
                f = 0;
                break;
            }
            if (f)
            {
                printf("%c", response[i]);
            }
        }
    }

    if (bytes_received == -1)
    {
        perror("recv");
    }
    close(sockfd);
    freeaddrinfo(result);
}
