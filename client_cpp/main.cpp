#include "client.h"
#include <iostream>

int main() 
{
    Client client;
    
    if(!client.Connect())
    {
        std::cerr << "Not connection to server!\n";
        return 1;
    }
    
    if(!client.Send("Hello!"))
    {
        std::cerr << "Send error\n";
        return 1;
    }

    auto answer = client.Receive();

    if(std::holds_alternative<bool>(answer))
    {
        std::cerr << "Receive error\n";
        return 1;
    }

    std::cout << std::get<std::string>(answer) <<std::endl;
    return 0;
}