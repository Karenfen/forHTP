#include "socket_wrap.h"
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>


SCTPsocket::SCTPsocket()
{
}

SCTPsocket::SCTPsocket(int new_sock) : m_socket(new_sock)
{
    if(m_socket < 0){
        throw std::runtime_error("socket creation error");
    }
}

SCTPsocket::~SCTPsocket()
{
    if(m_socket != -1)
    {
        close(m_socket);
    }
}

bool SCTPsocket::Init()
{
    if(m_socket != -1)
    {
        close(m_socket);
    }

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if(m_socket == -1)
    {
        return false;
    }

    return true;
}