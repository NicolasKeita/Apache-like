/*
** EPITECH PROJECT, 2019
** ServerUdpMultiThreadWrapper.cpp
** File description:
**
*/

#include <boost/asio.hpp>
#include <thread>
#include "ServerUdpMultiThreadWrapper.hpp"

//using boost::asio::ip::udp;

uti::network::ServerUdpMultiThreadWrapper::ServerUdpMultiThreadWrapper()
        : _online { false },
          _inbound_header {},
          _header_length { 8 },
          _inbound_data {},
          _handleMessageReceived { nullptr }
{}

/*
template<class T>
void uti::network::ServerUdpMultiThreadWrapper::turnOn(unsigned short port,
                                                       std::string (*handleMessageReceived)(const std::string &))
{
    _handleMessageReceived = handleMessageReceived;
    _socket = std::make_unique<udp::socket>(_io_context, udp::endpoint(udp::v4(), port));
    _online = true;


    while (true) {
        std::array<int8_t, 1024> data = {0};
        udp::endpoint sender_endpoint;
        T r = this->getReply<T>();
        size_t length = _socket->receive_from(boost::asio::buffer(data),
                                              sender_endpoint);

        std::thread thread_obj(&uti::network::ServerUdpMultiThreadWrapper::_handleRequest,
                               this,
                               sender_endpoint,
                               data,
                               length);
        thread_obj.detach();
        if (!_online)
            break;
    }
}
*/

/*
void uti::network::ServerUdpMultiThreadWrapper::sendMessageToTheLastestClient(const std::string &message)
{
    (void)message;
}
*/

/*
void uti::network::ServerUdpMultiThreadWrapper::_handleRequest(udp::endpoint &  sender_endpoint,
                                                               int8_t *         data,
                                                               size_t           length)
{
    std::string serverReplyToClient = _handleMessageReceived(std::string(data, length));
    _socket->send_to(boost::asio::buffer(serverReplyToClient,
                                         serverReplyToClient.size()),
                     sender_endpoint);
}*/
