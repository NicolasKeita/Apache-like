/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#pragma once
#include <string>
#include <iostream>

using ProtocolDataPacket = std::string;

namespace zia {
    class ProtocolHandler {
    public:
        //static std::string handleRequest(const std::string & incomingRequest) {(void)incomingRequest; return "";};
        static ProtocolDataPacket handleRequest(const ProtocolDataPacket & incomingRequest)
        {
            (void)incomingRequest;
            std::cout << "VOILA LE MESSAGE RECU : " << incomingRequest << std::endl;
            return ProtocolDataPacket();
        }
    };
}


