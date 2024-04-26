#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "asn1.h"

#define PORT 55665

void HandleConnectionInNewThread(void *socket);
void* HandleConnection(void *socket);

int main() {
    int listener;
    struct sockaddr_in addr;

    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (listener < 0) 
    {
        perror("Socket");
        exit(EXIT_FAILURE);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
        perror("Bind");
        close(listener);
        exit(EXIT_FAILURE);
    }

    if (listen(listener, 1) == -1) {
        perror("Listen");
        close(listener);
        exit(EXIT_FAILURE);
    }
    
    while (true) 
    {
        int sock = accept(listener, NULL, NULL);

        if (sock < 0) 
        {
            perror("Accept");
            continue;
        }

        HandleConnectionInNewThread((void*)&sock);
    }
    
    close(listener);
    
    return 0;
}

void HandleConnectionInNewThread(void *socket)
{
    pthread_t thread;

    if(pthread_create(&thread, NULL, HandleConnection, socket) != 0) {
        perror("Pthread create");
        close(*(int*)socket);
    }
    else
    {
        pthread_detach(thread);
    }
}

void* HandleConnection(void *sock)
{
    int socket = *(int*)sock;
    size_t bytes_received;
    char buf[buf_max_size];
    uint8_t *response;
    ssize_t response_len;

    bytes_received = sctp_recvmsg(socket, buf, buf_max_size, NULL, 0, NULL, NULL);

    if (bytes_received > 0)
    {        
        RRCConnectionRequest_t *rrc_request = NULL;
        
        if(RRCConnectionRequestDecoder(rrc_request, buf, bytes_received))
        {
            printf("RRC Connection Request decoded\n");
            RRCConnectionCompleteCoder(&response, &response_len);
            sleep(5);
            if(sctp_sendmsg(socket, (void *)response, response_len, NULL, 0, 0, 0, 0, 0, 0) > 0)
            {
                printf("Send answer\n");
            }
            else
            {
                perror("Send answer");
            }
        }
        else
        {
            perror("RRC Connection Request decod");
        }
    }
    close(socket);
}
