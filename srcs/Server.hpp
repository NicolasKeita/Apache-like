/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#pragma once

#include <string>
#include "my_network/BlockingMultiThread/Server/TCP/ServerTcpMultiThreadWrapper.hpp"
#include "ProtocolHandler.hpp"

namespace zia {
    template<class ProtocolDataPacket>
    class Server : uti::network::ServerTcpMultiThreadWrapper<ProtocolDataPacket> {
    public:
        explicit Server(const unsigned short int portToOpen, const uti::network::ProtocolType &protocolType)
            : uti::network::ServerTcpMultiThreadWrapper<ProtocolDataPacket> { protocolType }
        {
            this->turnOn(portToOpen, zia::ProtocolHandler::handleRequest);
        }
    private:
        ProtocolHandler protocol;
    };
}

