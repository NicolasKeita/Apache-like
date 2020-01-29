

#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include "BlockingMultiThread/ServerUdpMultiThreadWrapper.hpp"

using boost::asio::ip::udp;

// the return is the reply
std::string handleMessageReceived(const std::string &messageReceived)
{
    std::cerr << "[Message received] : " << messageReceived << "FIN" << std::endl;
    return messageReceived;
}

int main(int argc, char* argv[])
{
    std::cerr << "debut serv" << std::endl;
    try
    {
        uti::network::ServerUdpMultiThreadWrapper network;
        network.turnOn(42424, &handleMessageReceived);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}