/*
** EPITECH PROJECT, 2019
** INetworkCLientWrapper.hpp
** File description:
**
*/

#ifndef INETWORKCLIENTWRAPPER_HPP
#define INETWORKCLIENTWRAPPER_HPP

#include <string>

namespace uti::network {
    class IClientTcpAsyncWrapper {
        public:
            /*
             * When a new message from the server has come, handleMessageReceived() will be called.
             * The parameter of this function is the message received.
             * The return value is used to reply to the server,
             * return an empty string to not reply to the server.
             * The function is ran asynchronously.
             */
            virtual void connectToHost(const std::string &serverAddress,
                                       unsigned int port,
                                       std::string (*handleMessageReceived)(const std::string &)) = 0;
            virtual void sendMessage(const std::string &message) = 0;
    };
}

#endif //INETWORKCLIENTWRAPPER_HPP
