#include "client.h"
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <memory>



bool Client::Connect(const std::string &address, int port)
{
    if(!m_socket.Init())
    {
        std::cerr << "socket init error\n";
        return false;
    }

    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if(address == _LOCALHOST_)
    {
        servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
    }
    else
    {
        if(inet_pton(AF_INET, address.c_str(), &servaddr.sin_addr) <= 0) 
        {
            return false;
        }
    }

    return connect(*m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) != -1;
}

bool Client::Send(const std::string &message)
{
    if(m_socket.NotValid())
    {
        return false;
    }

    return sctp_sendmsg(*m_socket, (void*)(message.c_str()), message.length(), nullptr, 0, 0, 0, 0, 0, 0) > 0;
}

data Client::Receive()
{
    std::unique_ptr<char[]> m_buffer = std::make_unique<char[]>(buf_max_size);

    int bytes_received = sctp_recvmsg(*m_socket, m_buffer.get(), buf_max_size, NULL, 0, NULL, NULL);

    if(bytes_received > 0)
    {
        std::string str;
        str.copy(m_buffer.get(), bytes_received, 0);

        return str;
    }
    
    return false;
}
