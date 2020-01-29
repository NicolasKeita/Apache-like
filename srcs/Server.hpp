/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#pragma once

#include <string>
#include "my_network/BlockingMultiThread/Server/ServerUdpMultiThreadWrapper.hpp"

namespace zia {
    class Server : uti::network::ServerUdpMultiThreadWrapper {
    public:
        explicit Server(short int port);
        static std::string handleRequest(const std::string &) {return "";};
    };
}

