/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#pragma once
#include <string>

using ProtocolDataPacket = std::string;

namespace zia {
    class ProtocolHandler {
    public:
        //static std::string handleRequest(const std::string & incomingRequest) {(void)incomingRequest; return "";};
        static ProtocolDataPacket handleRequest(const ProtocolDataPacket & incomingRequest)
        {
            (void)incomingRequest;
            return ProtocolDataPacket();
        }
    };
}


