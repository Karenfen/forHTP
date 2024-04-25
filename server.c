#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "asn1.h"

#define PORT 55665

int main() {
    int sock, listener;
    struct sockaddr_in addr;
    char buf[buf_max_size];
    size_t bytes_received;
    uint8_t *response;
    ssize_t response_len;

    listener = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (listener < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
        perror("bind");
        close(listener);
        exit(EXIT_FAILURE);
    }

    if (listen(listener, 1) == -1) {
        perror("listen");
        close(listener);
        exit(EXIT_FAILURE);
    }
    
    while (true) 
    {
        sock = accept(listener, NULL, NULL);

        if (sock < 0) 
        {
            perror("accept");
            close(listener);
            exit(EXIT_FAILURE);
        }

        bytes_received = sctp_recvmsg(sock, buf, buf_max_size, NULL, 0, NULL, NULL);

        if (bytes_received > 0)
        {        
            RRCConnectionRequest_t *rrc_request = NULL;
            
            if(RRCConnectionRequestDecoder(rrc_request, buf, bytes_received))
            {
                printf("RRC Connection Request decoded\n");
                RRCConnectionCompleteCoder(&response, &response_len);

                if(sctp_sendmsg(sock, (void *)response, response_len, NULL, 0, 0, 0, 0, 0, 0) > 0)
                {
                    printf("Send answer\n");
                }
                else
                {
                    perror("send answer");
                }
            }
            else
            {
                printf("RRC Connection Request decod ERROR!\n");
            }
        }
        close(sock);
    }
    
    close(listener);
    
    return 0;
}
