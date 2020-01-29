/*
** EPITECH PROJECT, 2019
** IServerUdpMultiThreadWrapper.hpp
** File description:
**
*/

#ifndef MY_NETWORK_LIBRARY_ISERVERUDPMULTITHREADWRAPPER_HPP
#define MY_NETWORK_LIBRARY_ISERVERUDPMULTITHREADWRAPPER_HPP

namespace uti::network {
    class IServerUdpMultiThreadWrapper {
        public:
            virtual void turnOn(unsigned short port,
                                std::string (*handleMessageReceived)(const std::string &)) = 0;
            virtual void sendMessageToTheLastestClient(const std::string &message) = 0;
    };
}
#endif //MY_NETWORK_LIBRARY_ISERVERUDPMULTITHREADWRAPPER_HPP
