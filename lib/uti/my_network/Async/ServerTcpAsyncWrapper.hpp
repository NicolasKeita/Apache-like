/*
** EPITECH PROJECT, 2019
** ServerTcpAsyncWrapper.hpp
** File description:
**
*/

#ifndef SERVERWRAPPER_HPP
#define SERVERWRAPPER_HPP

#include <list>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/format.hpp>
#include "Async/IServerTcpAsyncWrapper.hpp"


using boost::asio::ip::tcp;

namespace uti::network {
    /*
     * Blocking server
     */
    class ServerTcpAsyncWrapper : public IServerTcpAsyncWrapper {
        public: // consider private
            class TcpConnection;
        public:
            ServerTcpAsyncWrapper();
            void turnOn(unsigned short port,
                          std::string (*handleMessageReceived)(const std::string &),
                          const std::string &welcomeMessage) override;
            void sendMessageToTheLastestClient(const std::string &message) override;

        private:
            void _startAccept();
            void _handleAccept(const boost::shared_ptr<TcpConnection> &new_connection,
                               const boost::system::error_code &error);

            std::list <boost::shared_ptr<TcpConnection>>  _all_clients;
            boost::asio::io_context                       _io_context;
            tcp::acceptor *                               _acceptor;
            std::string                                   _welcomeMessage;
            std::string (*_handleMessageReceived)(const std::string &);
        public: // consider private
            class TcpConnection : public boost::enable_shared_from_this<TcpConnection> {
                public:
                    TcpConnection(boost::asio::io_context& io_context,
                                  std::string welcomeMessage,
                                  std::string (*handleMessageReceived)(const std::string &));

                    tcp::socket& socket();
                    void start();
                    void sendMessage(const std::string &message);
                    void handleRead(const boost::system::error_code & e,
                                    std::size_t bytesTransferred);
                    void handleWrite();

                    tcp::socket                 socket_;
                    std::string                 _welcomeMessage;
                    boost::array<char, 8192>    _buffer;
                    std::string (*_handleMessageReceived)(const std::string &);
            };
    };
}

#endif //SERVERWRAPPER_HPP
