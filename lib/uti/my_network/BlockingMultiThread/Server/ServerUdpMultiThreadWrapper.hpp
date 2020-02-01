/*
** EPITECH PROJECT, 2019
** ServerUdpMultiThreadWrapper.hpp
** File description:
**
*/

#pragma once
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "IServerUdpMultiThreadWrapper.hpp"

namespace uti::network {
    template<class ProtocolDataPacket>
    class ServerUdpMultiThreadWrapper {
    public:
        ServerUdpMultiThreadWrapper()
                : _online { false },
                  _inbound_header {},
                  _header_length { 8 },
                  _inbound_data {},
                  _handleMessageReceived { nullptr }
        {}


        void turnOn(unsigned short int port, ProtocolDataPacket (*handleMessageReceived)(const ProtocolDataPacket &))
        {
            _handleMessageReceived = handleMessageReceived;
            using boost::asio::ip::udp;
            _socket = std::make_unique<udp::socket>(_io_context, udp::endpoint(udp::v4(), port));
            _online = true;

            while (true) {
                //std::array<int8_t, 1024> data = {0};
                std::pair<ProtocolDataPacket, udp::endpoint> clientMessage = this->getIncomingClientMessage();

                /*
                T o;
                udp::endpoint e;

                std::pair<T, udp::endpoint> r(o, e);
                 */

                //size_t length = _socket->receive_from(boost::asio::buffer(data), sender_endpoint);

                std::thread thread_obj(&uti::network::ServerUdpMultiThreadWrapper<ProtocolDataPacket>::_handleRequest,
                                       this,
                        //sender_endpoint,
                                       std::ref(clientMessage.second),
                                       clientMessage.first);
                //data,
                //length);
                thread_obj.detach();
                if (!_online)
                    break;
            }
        }


        std::pair<ProtocolDataPacket, boost::asio::ip::udp::endpoint> getIncomingClientMessage()
        {
            // Receive the header
            boost::asio::ip::udp::endpoint clientEndpoint;
            _socket->receive_from(boost::asio::buffer(_inbound_header), clientEndpoint);

            std::istringstream is(std::string(_inbound_header, _header_length));
            std::size_t inbound_data_size = 0;

            if (!(is >> std::hex >> inbound_data_size)) {
                std::cerr << "[CLientUdpMultiThread] Header is not valid : " << std::dec << inbound_data_size << std::endl;
                exit(31);
            }
            // Conversion hex to dec
            std::stringstream   stream;
            size_t              inbound_data_size_in_decimal = 0;

            stream << inbound_data_size;
            stream >> std::hex >> inbound_data_size_in_decimal;

            _inbound_data.resize(inbound_data_size_in_decimal);

            _socket->receive(boost::asio::buffer(_inbound_data));

            ProtocolDataPacket t;
            try {
                std::string archive_data(&_inbound_data[0], _inbound_data.size());
                std::istringstream archive_stream(archive_data);
                boost::archive::text_iarchive archive(archive_stream);
                archive >> t;
            }
            catch (const std::exception & e)
            {
                std::cerr << "[CLientUdpMultiThread] Unable to decode data.\n" << e.what() << std::endl;
                exit(34);
            }
            return std::pair(t, clientEndpoint);
        }


//        void sendMessageToTheLastestClient(const std::string &message) override;

    private:
        boost::asio::io_context _io_context;
        std::unique_ptr<boost::asio::ip::udp::socket> _socket;
        bool _online;
        char _inbound_header[8];
        size_t _header_length;
        std::vector<char> _inbound_data;

        ProtocolDataPacket (*_handleMessageReceived)(const ProtocolDataPacket &);

    private:
        void _handleRequest(const boost::asio::ip::udp::endpoint &sender_endpoint, ProtocolDataPacket data)
        {
            //(void)data;

            ProtocolDataPacket serverReplyToClient = _handleMessageReceived(data);

            std::string serverReplyToClient2 = "TODO(nicolas) change";

            _socket->send_to(boost::asio::buffer(serverReplyToClient2,
                                                 serverReplyToClient2.size()),
                             sender_endpoint);
        }
    };
}