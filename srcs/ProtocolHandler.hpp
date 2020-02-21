/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#pragma once
#include <string>
#include <iostream>
#include <openZia/Pipeline.hpp>
#include <openZia/Context.hpp>
#include <boost/serialization/string.hpp>

/*
 * Whatever protocol you are using. You MUST provide a serialize function to the class.
 * STL containers already have them in <boost/serialization/string.hpp>, replace "string" by your container.
 * I am using a text protocol using std::string in this HTTP server project
 *
 * For more details about the serialize function, see Boost Serialization documentation :
 * https://www.boost.org/doc/libs/1_72_0/libs/serialization/doc/serialization.html
 */
using ProtocolDataPacket = std::string;

namespace zia {
    class ProtocolHandler {
    public:
        explicit ProtocolHandler(oZ::Pipeline & pipeline) : _pipeline { pipeline } {};

        ProtocolDataPacket  onPacketReceived(const ProtocolDataPacket & incomingPacket, int fd);
        void                onAccept(int fd);
    private:
        std::string         _contextResponseToString(oZ::Context &context) const;

        std::string         _createResponse(oZ::Context & context) const;
        std::string         _createHeaderToSend(oZ::Context & context) const;
        std::string         _createBodyToSend(oZ::Context & context) const;

        oZ::Pipeline &  _pipeline;
    };
}


