/*
** EPITECH PROJECT, 2019
** ServerUdpMultiThreadWrapper.hpp
** File description:
**
*/

#pragma once
#include <memory>
#include <list>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <array>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/format.hpp>

namespace uti::network {
    enum ProtocolType {BINARY, TEXT};
    template<class ProtocolDataPacket>
    class ServerTcpMultiThreadWrapper {
    public:

        // It is recommanded to choose Binary as protocol type if you have control over the client
        explicit ServerTcpMultiThreadWrapper(ProtocolType protocolType)
                : _online { false },
                  _inbound_header {},
                  _header_length { 8 },
                  _inbound_data {},
                  _handleMessageReceived { nullptr },
                  _protocolType { protocolType }
        {}


        void turnOn(unsigned short int port, ProtocolDataPacket (*handleMessageReceived)(const ProtocolDataPacket &))
        {
            _handleMessageReceived = handleMessageReceived;
            using boost::asio::ip::tcp;
            _sockets.push_back(tcp::socket(_io_context));
            _acceptor = std::make_unique<tcp::acceptor>(_io_context, tcp::endpoint(tcp::v4(), port));
            _online = true;

            while (true) {
                _acceptor->accept(_sockets.back());
                const ProtocolDataPacket clientMessage = this->getIncomingClientMessage(_sockets.back());
                std::thread thread_obj(&uti::network::ServerTcpMultiThreadWrapper<ProtocolDataPacket>::_handleRequest,
                                       this,
                                       std::ref(_sockets),
                                       std::ref(_sockets.back()), // TODO: should be fine to remove the extra param
                                       std::ref(clientMessage));
                thread_obj.detach();
                if (!_online)
                    break;
            }
        }


        ProtocolDataPacket getIncomingClientMessage(boost::asio::ip::tcp::socket &socket)
        {
            if (_protocolType == BINARY) {
                // Receive the header
                boost::asio::ip::tcp::endpoint clientEndpoint;
                socket.receive(boost::asio::buffer(_inbound_header));

                std::istringstream is(std::string(_inbound_header, _header_length));
                std::size_t inbound_data_size = 0;

                if (!(is >> std::hex >> inbound_data_size)) {
                    std::cerr << "[CLientUdpMultiThread] Header is not valid : " << std::dec << inbound_data_size
                              << std::endl;
                    exit(31);
                }
                // Conversion hex to dec
                std::stringstream stream;
                size_t inbound_data_size_in_decimal = 0;

                stream << inbound_data_size;
                stream >> std::hex >> inbound_data_size_in_decimal;

                _inbound_data.resize(inbound_data_size_in_decimal);

                socket.receive(boost::asio::buffer(_inbound_data));

                ProtocolDataPacket t;
                try {
                    std::string archive_data(&_inbound_data[0], _inbound_data.size());
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> t;
                }
                catch (const std::exception &e) {
                    std::cerr << "[CLientUdpMultiThread] Unable to decode data.\n" << e.what() << std::endl;
                    exit(34);
                }
                return t;
            }
            else // _protocolType == TEXT
            {
                boost::array<char, 8089> incomingData {};
                socket.receive(boost::asio::buffer(incomingData));
                std::string incomingDataCast(incomingData.begin(), incomingData.end());
                return static_cast<ProtocolDataPacket>(incomingDataCast);
            }
        }


//        void sendMessageToTheLastestClient(const std::string &message) override;

    private:
        boost::asio::io_context                         _io_context;
        //std::unique_ptr<boost::asio::ip::tcp::socket>   _socket;
        std::list<boost::asio::ip::tcp::socket>         _sockets;
        std::unique_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
        bool                _online;
        char                _inbound_header[8];
        size_t              _header_length;
        std::vector<char>   _inbound_data;
        ProtocolDataPacket (*_handleMessageReceived)(const ProtocolDataPacket &);
        ProtocolType        _protocolType;

    private:
        //void _handleRequest(const boost::asio::ip::tcp::endpoint &sender_endpoint, ProtocolDataPacket data)
        void _handleRequest(std::list<boost::asio::ip::tcp::socket> &sockets, boost::asio::ip::tcp::socket &socket, const ProtocolDataPacket &data)
        {
            //(void)data;
            (void)sockets;

            const ProtocolDataPacket serverReplyToClient = _handleMessageReceived(data);
            sendMessage(socket, serverReplyToClient);
            /*

            std::string serverReplyToClient2 = "TODO(nicolas) change";

            _socket->send_to(boost::asio::buffer(
                    serverReplyToClient), TODO (nicolas) Need to serialize this before sending it

                    serverReplyToClient2,
                    serverReplyToClient2.size()),

                             sender_endpoint);
            */
        }

        void sendMessage(boost::asio::ip::tcp::socket & socket, const ProtocolDataPacket & message)
        {
            if (_protocolType == BINARY) {
                // Serialization
                std::ostringstream archive_stream;
                boost::archive::text_oarchive archive(archive_stream);
                archive << message;
                std::string message_serialized = archive_stream.str();

                // Header creation
                std::ostringstream header_stream;
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
                socket.send(boost::asio::buffer(header));
                //_socket->send_to(boost::asio::buffer(message_serialized), *_endpoints.begin());
                socket.send(boost::asio::buffer(message_serialized));
                //_socket->send_to(buffers, *_endpoints.begin()); // TODO : send it only once (merge header + message)
            }
            else if (_protocolType == TEXT) {
                auto messageCast = static_cast<std::string>(message);
                socket.send(boost::asio::buffer(messageCast));
            }
        }

    };
}