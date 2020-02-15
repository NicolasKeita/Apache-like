/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#pragma once

#include <string>
#include <utility>
#include <functional>
#include "my_network/BlockingMultiThread/Server/TCP/ServerTcpMultiThreadWrapper.hpp"
#include "ProtocolHandler.hpp"

namespace zia {
    template<class ProtocolDataPacket>
    class Server : uti::network::ServerTcpMultiThreadWrapper<ProtocolDataPacket> {
    public:
        explicit Server(const unsigned short int portToOpen,
                        const uti::network::ProtocolType &protocolType)
                : uti::network::ServerTcpMultiThreadWrapper<ProtocolDataPacket> { protocolType },
                  _pipeline { "../lib", "./"},
                  _protocolHandler { _pipeline }
        {
            _pipeline.loadModules();
            std::cout << "[DEBUG ZIA] Modules loaded !" << std::endl;
            this->turnOn(portToOpen,
                         std::bind(&zia::ProtocolHandler::onPacketReceived,
                                   _protocolHandler,
                                   std::placeholders::_1)
            );
        }
    private:
        oZ::Pipeline    _pipeline;
        ProtocolHandler _protocolHandler;
    };
}

