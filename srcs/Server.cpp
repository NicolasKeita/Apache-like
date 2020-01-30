/*
** Creation Year : 2020 ; Month : 01 ; Day : 29.
*/

#include "Server.hpp"

zia::Server::Server(short int port)
{
    this->turnOn(port, &zia::ProtocolHandler::handleRequest);
}