#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include "asn1.h"

#define SERVER_PORT 55665

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    uint8_t *request;
    ssize_t request_len;
    uint8_t response[buf_max_size];
    ssize_t bytes_received;

    RRCConnectionRequestCoder(&request, &request_len);
    
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (sockfd == -1) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) 
    {
        perror("Connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (sctp_sendmsg(sockfd, (void *)request, request_len, NULL, 0, 0, 0, 0, 0, 0) == -1) 
    {
        perror("SCTP send message");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    bytes_received = sctp_recvmsg(sockfd, response, buf_max_size, NULL, 0, NULL, NULL);

    if (bytes_received > 0) 
    {
        RRCConnectionSetup_t *rrc_response = NULL;

        if(RRCConnectionCompleteDecoder(rrc_response, response, bytes_received))
        {
            printf("RRC Connection Setup decoded\n");
        }
        else
        {
            perror("RRC connection complete decode");
        }
    }

    close(sockfd);

    return 0;
}
