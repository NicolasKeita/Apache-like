/*
** EPITECH PROJECT, 2019
** ServerUdpMultiThreadWrapper.cpp
** File description:
**
*/

#include <boost/asio.hpp>
#include <thread>
#include "ServerUdpMultiThreadWrapper.hpp"

using boost::asio::ip::udp;

uti::network::ServerUdpMultiThreadWrapper::ServerUdpMultiThreadWrapper()
        : _handleMessageReceived { nullptr }
{}

void uti::network::ServerUdpMultiThreadWrapper::turnOn(unsigned short port,
                                                       std::string (*handleMessageReceived)(const std::string &))
{
    _handleMessageReceived = handleMessageReceived;
    _socket = std::make_unique<udp::socket>(_io_context, udp::endpoint(udp::v4(), port));


    while (true) {
        char data[1024];
        udp::endpoint sender_endpoint;
        size_t length = _socket->receive_from(
                boost::asio::buffer(data, 1024), sender_endpoint);

        std::thread thread_obj(&uti::network::ServerUdpMultiThreadWrapper::_handleRequest,
                               this,
                               sender_endpoint,
                               data,
                               length);
        thread_obj.detach();
    }
}

void uti::network::ServerUdpMultiThreadWrapper::sendMessageToTheLastestClient(const std::string &message)
{
    (void)message;
}

void uti::network::ServerUdpMultiThreadWrapper::_handleRequest(udp::endpoint sender_endpoint, char *data, size_t length)
{
    std::string serverReplyToClient =
            _handleMessageReceived(std::string(data, length));
    _socket->send_to(boost::asio::buffer(serverReplyToClient,
                                         serverReplyToClient.size()),
                     sender_endpoint);
}
