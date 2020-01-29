
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "Async/ClientTcpAsyncWrapper.hpp"

std::string handleMessageReceived(const std::string &messageReceived)
{
    std::cerr << "[DEBUG] Message received:" << messageReceived << "FIN" << std::endl;
    if (messageReceived == "CD")
        return "EF";
    if (messageReceived == "GH")
        return "IJ";
    if (messageReceived == "fast")
        return "for you";
    return "";
}

int main()
{
    uti::network::ClientTcpAsyncWrapper client;
    try {
        client.connectToHost("0.0.0.0", 42424, &handleMessageReceived);
        client.sendMessage("AB");
        std::cerr << "[DEBUG] Debut de la loop" << std::endl;
        bool did = false;

        while (true) {
            usleep(200000);
            if (!did) {
                client.sendMessage("too");
                did = true;
            }
            std::cerr << "[DEBUG] One loop done" << std::endl;
        }
    }
    catch (std::exception &e) {
        std::cerr << "[DEBUG]" << e.what() << std::endl;
    }
    return 0;
}