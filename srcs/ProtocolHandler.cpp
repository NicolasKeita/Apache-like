/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#include <utility>
#include <sstream>
#include <fstream>
#include "ProtocolHandler.hpp"

void zia::ProtocolHandler::onAccept(int fd)
{
    oZ::ByteArray byteArray {};
    oZ::Context context(oZ::Packet(std::move(byteArray), oZ::Endpoint(), fd));
    std::cout << "[DEBUG ZIA] OnAccept() called" << std::endl;

    context.getResponse().getReason() = "BeforeParse";
    context.setState(oZ::State::BeforeParse);

    _pipeline.runPipeline(context);

}

ProtocolDataPacket zia::ProtocolHandler::onPacketReceived(const ProtocolDataPacket &incomingPacket, int fd)
{
    // Cast to ByteArray cuz "oZ API" requires a std::vector<int8_t> == ByteArray but I use std::string
    oZ::ByteArray byteArray(incomingPacket.begin(), incomingPacket.end());

    oZ::Context context(oZ::Packet(std::move(byteArray), oZ::Endpoint(), fd));
    context.getResponse().getReason() = "Interpret";
    context.setState(oZ::State::Interpret);
    _pipeline.runPipeline(context);

    if (context.isCompleted())
        return "";
    else {
        return _contextResponseToString(context);
    }
}

std::string zia::ProtocolHandler::_contextResponseToString(oZ::Context &context) const
{
    std::string header =
            context.getResponse().getHeader().get("version") + " " +
            context.getResponse().getHeader().get("statut") + " " +
            context.getResponse().getHeader().get("code") + "\n" +
            context.getResponse().getHeader().get("content-type") + "\n" +
            context.getResponse().getHeader().get("content-length") + "\n";
    std::string body = context.getResponse().getBody();

    return header + "\n" + body;
}
