/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#pragma once

#include <string>
#include "my_network/BlockingMultiThread/Server/ServerUdpMultiThreadWrapper.hpp"
#include "ProtocolHandler.hpp"

namespace zia {
    template<class ProtocolDataPacket>
    class Server : uti::network::ServerUdpMultiThreadWrapper<ProtocolDataPacket> {
    public:
        explicit Server(unsigned short int portToOpen)
        {
            this->turnOn(portToOpen, zia::ProtocolHandler::handleRequest);
        }
    private:
        ProtocolHandler protocol;
    };
}

