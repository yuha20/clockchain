//
// Created by yuling on 12/3/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Blockchain.h"
#include <ctime>
#include <assert.h>

const int SOCKET_BUFFER=40960;
void error(const char *msg) {
    perror(msg);
    exit(1);
}
void print_usage()
{
    printf("clockchain -- clock chain server "
           "\n"
           "Usage:\n"
           "  clockchain [options...]\n"
           "\n"
           "Options:\n"
           "  -h, --help\n"
           "        Print this message\n"
           "  -p, --port <int>\n"
    );
}
int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno=-1;
    socklen_t clilen;
    struct option long_options[] = {
            // These options don't set a flag
            {"help",         no_argument,       NULL, 'h'},
            {"port",     required_argument, NULL, 'p'},
            {NULL, 0,                           NULL, 0}
    };

    int index;
    while (1) {
        index = 0;
        int c = getopt_long(argc, argv, "h:p:", long_options, &index);

        if (c == -1)
            break;

        if (c == 0 && long_options[index].flag == 0)
            c = long_options[index].val;

        switch (c) {
            case 0:
                /* Flag is automatically set */
                break;
            case 'h':
                print_usage();
                exit(0);

            case 'p':
                portno = atoi(optarg);
                break;
            case '?':
                printf("Use -h or --help for help.\n");
                exit(0);
            default:
                exit(1);
        }
    }
    if(portno == -1)
    {
        print_usage();
        exit(0);
    }

    char buffer[SOCKET_BUFFER];

    // a smart pointer for Blockchain object which will create a timer thread to emit a timer block request into a queue every minute
    // and a minging thread to process the minging requests in the queue
    std::unique_ptr<Blockchain>  bChain( new Blockchain());
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    int n;
    while (true) {
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        bzero(buffer, SOCKET_BUFFER);
        n = read(newsockfd, buffer, SOCKET_BUFFER);
        string response=bChain->ReceivedRequest(buffer);
        if (n < 0) error("ERROR reading from socket");
        n = write(newsockfd, response.c_str(), response.length());
        if (n < 0) error("ERROR writing to socket");
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}