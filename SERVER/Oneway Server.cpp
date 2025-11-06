
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <math.h>
#include <thread>
#include <vector>
#include <format>

#include "Client.h"



#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    std::vector<Client> clients;
    
    std::string addr;
    std::string port;

    std::cout << "IP: ";
    std::getline(std::cin, addr);
    std::cout << "Port: ";
    std::getline(std::cin, port);

    std::cout << addr << " | " << port << "\n";
    

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    int sockfd;
    struct addrinfo hints, * servinfo, * p;
    int rv;
    int numbytes;
    struct sockaddr their_addr;
    char buf[MAXBUFLEN];
    char toSplit[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(addr.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        //std::cout << rv;
        fprintf(stderr, "getaddrinfo: %ls\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            closesocket(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);


    printf("listener: waiting to recv...\n");
    while (buf != "DISCONNECT") {
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
            (struct sockaddr*)&their_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(1);
        }

        //printf("listener: got packet from %s\n", &their_addr);
        //printf("listener: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        //printf("listener: packet contains \"%s\"\n", buf); 

        char* returnStr = { 0 };
        bool needsToReturnSomething;
        if(buf[0] == 'U') {
            std::string username;
            std::string x;
            std::string y;
            std::string angle;

            int i = 2;
            for (i; i < strlen(buf); i++) {
                if (buf[i] == ':') {
                    i++;
                    break;
                }
                username.push_back(buf[i]);
            }
            for (i; i < strlen(buf); i++) {
                if (buf[i] == ':') {
                    i++;
                    break;
                }
                x.push_back(buf[i]);
            }
            for (i; i < strlen(buf); i++) {
                if (buf[i] == ':') {
                    i++;
                    break;
                }
                y.push_back(buf[i]);
            }
            for (i; i < strlen(buf); i++) {
                if (buf[i] == '\0') {
                    i++;
                    break;
                }
                angle.push_back(buf[i]);
            }

            //std::cout << x << " " << y << " " << angle << "\n";
            float X = std::stof(x);
            float Y = std::stof(y);
            float ANGLE = std::stof(angle);
            //std::cout << username << "\n";
            std::string positions;
            bool exists = false;
            for (Client& client : clients) {
                if (client.username == username.data()) {

                    client.x = X;
                    client.y = Y;
                    client.angle = ANGLE;
                    exists = true;
                }
                else {

                    std::string clientInfo = std::format("P:{}:{}:{}:{}|", client.username, client.x, client.y, client.angle);
                    positions = positions + clientInfo;
                }
            }

            positions += '!';

            if (not exists) {
                clients.push_back(Client(username, X, Y, ANGLE));
                std::cout << "New client connected: " << username << "\n";
            }

            char* POSITIONS = positions.data();
            returnStr = POSITIONS;
            needsToReturnSomething = true;
        }
        
        

        

        
        std::cout << "Return String: " << returnStr << "\n";
        if ((numbytes = sendto(sockfd, returnStr, strlen(returnStr), 0, &their_addr, sizeof(their_addr))) == -1) {
            std::cout << "ERROR" << errno;
            exit(1);
        }

        
        
        



    }
    

    closesocket(sockfd);
    WSACleanup();

    

    return 0;
}

