/*
** EPITECH PROJECT, 2019
** IClientUdpMultiThreadWrapper.hpp
** File description:
**
*/

#ifndef MY_NETWORK_LIBRARY_ICLIENTUDPMULTITHREADWRAPPER_HPP
#define MY_NETWORK_LIBRARY_ICLIENTUDPMULTITHREADWRAPPER_HPP

#include <boost/any.hpp>

namespace uti::network {
    class IClientUdpMultiThreadWrapper {
        public:
            virtual void setServer(const std::string &serverAddress, unsigned short port) = 0;
            // template<class T> void sendMessage(const T & message) Templates cannot appear in interfaces
            // template<class T> T getReply() Templates cannot appear in interfaces
            virtual void stop() = 0;
    };
}

#endif //MY_NETWORK_LIBRARY_ICLIENTUDPMULTITHREADWRAPPER_HPP
