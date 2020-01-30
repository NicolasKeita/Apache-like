/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#include "Server.hpp"
#include "ProtocolDataPacket.hpp"

zia::Server::Server(short int portToOpen)
{
    this->turnOn<ProtocolDataPacket>(portToOpen, zia::ProtocolHandler::handleRequest);
}