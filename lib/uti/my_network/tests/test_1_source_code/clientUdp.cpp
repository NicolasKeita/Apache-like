


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include "BlockingMultiThread/ClientUdpMultiThreadWrapper.hpp"

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::cerr << "debut client" << std::endl;
    try {
        uti::network::ClientUdpMultiThreadWrapper network;
        network.setServer("0.0.0.0", 42424);
        network.sendMessage("Salut ici le main");

        std::string str = network.getReply();
        std::cout << "reply is NEW : " << str << "FIN size: "<< str.size() << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "[Client UDP Test] Exception: " << e.what() << std::endl;
    }
}
