/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#pragma once

#include <string>
#include "my_network/BlockingMultiThread/Server/ServerUdpMultiThreadWrapper.hpp"
#include "ProtocolHandler.hpp"

namespace zia {
    class Server : uti::network::ServerUdpMultiThreadWrapper {
    public:
        explicit Server(short int port);
    private:
        ProtocolHandler protocol;
    };
}

