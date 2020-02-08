/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#include <utility>
#include <sstream>
#include "ProtocolHandler.hpp"

ProtocolDataPacket zia::ProtocolHandler::onPacketReceived(const ProtocolDataPacket &incomingPacket)
{
    // Cast to ByteArray cuz "oZ API" requires a std::vector<int8_t> == ByteArray but I use std::string
    oZ::ByteArray byteArray(incomingPacket.begin(), incomingPacket.end());

    oZ::Context context(oZ::Packet(std::move(byteArray), oZ::Endpoint()));
    _pipeline.runPipeline(context);

    std::string responseBody = _createBodyToSend(context);
    std::string responseHeader = _createHeaderToSend(context);
    return responseHeader + "\n" + responseBody;
}

std::string zia::ProtocolHandler::_createHeaderToSend(oZ::Context &context) const
{
    oZ::HTTP::Version version = context.getResponse().getVersion(); // TODO fix this unused operation
    version.majorVersion = 1;
    version.minorVersion = 1;

    oZ::HTTP::Code code = context.getResponse().getCode(); // TODO fix this unused operation
    code = oZ::HTTP::Code::OK;

    oZ::HTTP::Reason reason = context.getResponse().getReason();
    reason = "OK";

    std::ostringstream versionToString;
    versionToString << "HTTP/" << version.majorVersion << "." << version.minorVersion;
    context.getResponse().getHeader().set("version",  versionToString.str());

    context.getResponse().getHeader().set("statut",  reason);
    context.getResponse().getHeader().set("code", static_cast<int8_t>(code));

    context.getResponse().getHeader().set("content-type", "Content-Type: text/plain");
    context.getResponse().getHeader().set("content-length", "Content-Length: 12");
    return (
            context.getResponse().getHeader().get("version") + " " +
            context.getResponse().getHeader().get("statut") + " " +
            context.getResponse().getHeader().get("code") + "\n" +
            context.getResponse().getHeader().get("content-type") + "\n" +
            context.getResponse().getHeader().get("content-length") + "\n"
    );
}

std::string zia::ProtocolHandler::_createBodyToSend(oZ::Context &context) const
{
    std::string body = "Hello worlE\n";
    context.getResponse().getBody() = body;
    return context.getResponse().getBody();
}