/*
** EPITECH PROJECT, 2019
** ClientTcpAsyncWrapper.cpp
** File description:
**
*/

#include <iostream>
#include <string>
#include <deque>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include "ClientTcpAsyncWrapper.hpp"
#include "MyStrTok.hpp"

uti::network::ClientTcpAsyncWrapper::ClientTcpAsyncWrapper()
        : _buf {},
          _connected { false },
          _handleMessageReceived { nullptr }
{}

uti::network::ClientTcpAsyncWrapper::~ClientTcpAsyncWrapper()
{
    _t->join();
}

void uti::network::ClientTcpAsyncWrapper::connectToHost(const std::string &serverAddress,
                                                        unsigned int port,
                                                        std::string (*handleMessageReceived)(const std::string &))
{
    _resolver = std::make_unique<boost::asio::ip::tcp::resolver>(_io_context);
    _endpoints = _resolver->resolve(serverAddress, std::to_string(port));
    _socket = std::make_unique<boost::asio::ip::tcp::socket>(_io_context);
    _handleMessageReceived = handleMessageReceived;
    boost::asio::connect(*_socket, _endpoints);
    _t = std::make_unique<std::thread>([&](){ _io_context.run();});
    _socket->async_read_some(boost::asio::buffer(_buf),
                             boost::bind(&ClientTcpAsyncWrapper::_handleRead,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
    _connected = true;
}

void uti::network::ClientTcpAsyncWrapper::sendMessage(const std::string &message_origin)
{
    if (!_connected)
        return;
    std::string message = message_origin;
    if (!message.empty()) {
        if (message.back() != '\n')
            message += '\n';
        bool write_in_progress = !_messagesToSend.empty();
        _messagesToSend.push_back(message);
        if (!write_in_progress) {
            _do_write();
        }
    }
}

void uti::network::ClientTcpAsyncWrapper::_do_write()
{
    _socket->write_some(boost::asio::buffer(_messagesToSend.front()));
    _messagesToSend.pop_front();
}

void uti::network::ClientTcpAsyncWrapper::_handleRead(const boost::system::error_code & error,
                                                      std::size_t bytesTransferred)
{
    if (error == boost::asio::error::eof) {
        std::cerr << "[DEBUG] Connection closed" << std::endl;
        _socket->close();
        return; // Connection closed cleanly by peer.
    }
    else if (error)
        throw boost::system::system_error(error);
    std::string messageReceived;
    std::copy(_buf.begin(), _buf.begin()+ bytesTransferred, std::back_inserter(messageReceived));
    std::vector<std::string> messages;
    uti::myStrTok(messageReceived, messages, "\n");
    for (const std::string &msg : messages) {
        std::string reply = _handleMessageReceived(msg);
        if (!reply.empty()) {
            if (reply.back() != '\n')
                reply += '\n';
            sendMessage(reply);
        }
    }
    _socket->async_read_some(boost::asio::buffer(_buf),
                             boost::bind(&ClientTcpAsyncWrapper::_handleRead,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
