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
                std::cerr << "GOT A MESSAGE ! : " << std::endl;
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
            sendMessage(serverReplyToClient, sender_endpoint);
            /*

            std::string serverReplyToClient2 = "TODO(nicolas) change";

            _socket->send_to(boost::asio::buffer(
                    serverReplyToClient), TODO (nicolas) Need to serialize this before sending it

                    serverReplyToClient2,
                    serverReplyToClient2.size()),

                             sender_endpoint);
            */
        }

        void sendMessage(const ProtocolDataPacket & message, const boost::asio::ip::udp::endpoint &sender_endpoint)
        {
            // Serialization
            std::ostringstream archive_stream;
            boost::archive::text_oarchive archive(archive_stream);
            archive << message;
            std::string message_serialized = archive_stream.str();

            // Header creation
            std::ostringstream  header_stream;
            header_stream << std::setw(static_cast<int>(_header_length)) \
                << std::hex << message_serialized.size();
            if (!header_stream || header_stream.str().size() != _header_length) {
                std::cerr << "[CLientUdpMultiThread] Serialization Object went wrong" << std::endl;
                exit(84);
            }
            std::string header = header_stream.str();

            // Merge
            std::vector<boost::asio::const_buffer> buffers;
            buffers.emplace_back(boost::asio::buffer(header));
            buffers.emplace_back(boost::asio::buffer(message_serialized));

            // Sending a long serialized message
            //_socket->send_to(boost::asio::buffer(header), *_endpoints.begin());
            _socket->send_to(boost::asio::buffer(header), sender_endpoint);
            //_socket->send_to(boost::asio::buffer(message_serialized), *_endpoints.begin());
            _socket->send_to(boost::asio::buffer(message_serialized), sender_endpoint);
            //_socket->send_to(buffers, *_endpoints.begin()); // TODO : send it only once (merge header + message)
        }

    };
}