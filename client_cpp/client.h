#include <socket_wrap.h>
#include <variant>
#include <string>

#define _LOCALHOST_ "localhost"
const size_t buf_max_size = 1024;
using data =  std::variant<bool, std::string>;;

class Client
{
private:
    SCTPsocket m_socket;

public:
    bool Connect(const std::string &address, int port);
    inline bool Connect(int port = DEFAULT_PORT) { return Connect(std::string(_LOCALHOST_), port); };
    bool Send(const std::string& message);
    data Receive();
};