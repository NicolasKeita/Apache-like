/*
** EPITECH PROJECT, 2019
** ClientTcpAsyncWrapper.hpp
** File description:
**
*/

#ifndef R_TYPE_VIDEO_GAME_CLIENTWRAPPER_HPP
#define R_TYPE_VIDEO_GAME_CLIENTWRAPPER_HPP

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <deque>
#include <string>
#include <memory>
#include "IClientTcpAsyncWrapper.hpp"

namespace uti::network {
    class ClientTcpAsyncWrapper : public IClientTcpAsyncWrapper {
        public:
            ClientTcpAsyncWrapper();
            ~ClientTcpAsyncWrapper();
            void connectToHost(const std::string &serverAddress,
                               unsigned int port,
                               std::string (*handleMessageReceived)(const std::string &)) override;
            void sendMessage(const std::string &message) override;

        private:
            void _handleRead(const boost::system::error_code & e,
                             std::size_t bytesTransferred);
            void _do_write();

        public:
            boost::asio::io_context                         _io_context;
        private:
            std::unique_ptr<boost::asio::ip::tcp::resolver> _resolver;
            boost::asio::ip::tcp::resolver::results_type    _endpoints;
            std::unique_ptr<boost::asio::ip::tcp::socket>   _socket;
            boost::array<char, 128>                         _buf;
            std::deque<std::string>                         _messagesToSend;
            std::unique_ptr<std::thread>                     _t;
            bool                                            _connected;
            std::string (*_handleMessageReceived)(const std::string &);
    };
}


#endif //R_TYPE_VIDEO_GAME_CLIENTWRAPPER_HPP
