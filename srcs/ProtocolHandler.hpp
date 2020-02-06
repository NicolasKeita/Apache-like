/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#pragma once
#include <string>
#include <iostream>
#include <openZia/Pipeline.hpp>
#include <boost/serialization/string.hpp>

/*
 * Whatever protocol you are using. You MUST provide a serialize function to the class.
 * STL containers already have them in <boost/serialization/string.hpp>, replace "string" by your container.
 *
 * For more detail about the serialize function see Boost Serialization documentation :
 * https://www.boost.org/doc/libs/1_72_0/libs/serialization/doc/serialization.html
 */
using ProtocolDataPacket = std::string;

namespace zia {
    class ProtocolHandler {
    public:
        ProtocolDataPacket onPacketReceived(const ProtocolDataPacket & incomingPacket)
        {
            std::cout << "VOILA LE MESSAGE RECU : " << incomingPacket << std::endl;
            return ProtocolDataPacket();
        }
    };
}


