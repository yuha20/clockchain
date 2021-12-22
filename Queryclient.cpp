//
// Created by yuling on 12/3/20.
//



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctime>
#include <string>
#include <iostream>
#include "json/json.h"
const int SOCKET_BUFFER=40960;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
//get the block chain request
std::string inqueryRequest()
{
    std::string jsonStr;
    Json::Value root, fruit, mail;
    Json::StreamWriterBuilder writerBuilder;
    std::ostringstream os;

    writerBuilder.settings_["indentation"] = "\t";

    root["op"] = "inqury";
    root["clientid"] = 1;
    root["token"] = "xxxxxxxxxxxxxxxxxxxxxxx";
    root["time"]=time(NULL);

    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os);
    jsonStr = os.str();
    return jsonStr;

}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[SOCKET_BUFFER];
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    std::string request;
    request= inqueryRequest();
    n = write(sockfd,request.c_str(),request.length());
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer,SOCKET_BUFFER);
    n = read(sockfd,buffer,SOCKET_BUFFER);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}