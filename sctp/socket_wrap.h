#define DEFAULT_PORT 55665

struct sockaddr_in;

class SCTPsocket
{
private:
    int m_socket = -1;

public:
    SCTPsocket();
    SCTPsocket(int new_sock);
    SCTPsocket(const SCTPsocket& other) = delete;
    SCTPsocket(SCTPsocket&& other) = delete;
    ~SCTPsocket();

    SCTPsocket& operator=(const SCTPsocket& other) = delete;
    SCTPsocket& operator=(SCTPsocket&& other) = delete; 

    inline int operator*() const {return m_socket;}

    bool Init();
    inline bool NotValid() const {return m_socket == -1;}
};