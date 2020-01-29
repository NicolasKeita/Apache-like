/*
** EPITECH PROJECT, 2019
** ClientUdpMultiThreadWrapper.cpp
** File description:
**
*/

#include "ClientUdpMultiThreadWrapper.hpp"

uti::network::ClientUdpMultiThreadWrapper::ClientUdpMultiThreadWrapper()
        : _resolver { _io_context },
          _serverAddress { "-1" },
          _port { 0 },
          _serverSet { false },
          _header_length { 8 },
          inbound_header {}
{}

void uti::network::ClientUdpMultiThreadWrapper::setServer(const std::string &serverAddress,
                                                          unsigned short port)
{
    _serverAddress = serverAddress;
    _port = port;
    using boost::asio::ip::udp;
    _socket = std::make_unique<udp::socket>(_io_context,
                                            udp::endpoint(udp::v4(), port)); // Change port to 0 may fix a mysterious port bug
    _endpoints = _resolver.resolve(udp::v4(),
                                   _serverAddress,
                                   std::to_string(_port));
    _serverSet = true;
}

void uti::network::ClientUdpMultiThreadWrapper::stop()
{
    _socket->shutdown(boost::asio::ip::udp::socket::shutdown_both);
}