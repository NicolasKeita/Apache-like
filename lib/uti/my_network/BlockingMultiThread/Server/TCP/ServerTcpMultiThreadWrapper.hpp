/*
** EPITECH PROJECT, 2019
** ServerUdpMultiThreadWrapper.hpp
** File description:
**
*/

#pragma once
#include <memory>
#include <array>
#include <list>
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/format.hpp>

namespace uti::network {
    enum ProtocolType {BINARY, TEXT};
    template<class ProtocolDataPacket>
    class ServerTcpMultiThreadWrapper {
    public:

        // It is recommended to choose Binary as protocol type if you have control over the client
        explicit ServerTcpMultiThreadWrapper(ProtocolType protocolType)
                 : _online { false },
                  _inbound_header {},
                  _header_length { 8 },
                  _inbound_data {},
                  _protocolType { protocolType }
        {}

        void turnOn(const unsigned short int port,
                    std::function<void(int)> onAccept,
                    std::function<ProtocolDataPacket(const ProtocolDataPacket &, int)> onPacketReceived)
        {
            using boost::asio::ip::tcp;
            _sockets.push_back(tcp::socket(_io_context));

            _acceptor = std::make_unique<tcp::acceptor>(_io_context, tcp::endpoint(tcp::v4(), port));
            _online = true;

            while (true) {
                _acceptor->accept(_sockets.back());
                onAccept(_sockets.back().native_handle());
                const ProtocolDataPacket clientMessage = this->getIncomingClientMessage(_sockets.back());
                std::thread thread_obj(&uti::network::ServerTcpMultiThreadWrapper<ProtocolDataPacket>::_handleRequest,
                                       this,
                                       std::ref(_sockets),
                                       std::ref(_sockets.back()), // TODO: remove this extra param
                                       clientMessage,
                                       onPacketReceived
                                       );
                thread_obj.detach();
                if (!_online)
                    break;
                _sockets.push_back(tcp::socket(_io_context));
            }
        }


        ProtocolDataPacket getIncomingClientMessage(boost::asio::ip::tcp::socket &socket)
        {
            if (_protocolType == BINARY) {
                /* Receive the header */
                socket.receive(boost::asio::buffer(_inbound_header));

                std::istringstream is(std::string(_inbound_header, _header_length));
                std::size_t inbound_data_size = 0;

                if (!(is >> std::hex >> inbound_data_size)) {
                    std::cerr << "[CLientUdpMultiThread] Header is not valid : " << std::dec << inbound_data_size
                              << std::endl;
                    exit(31);
                }
                /* Conversion hex to dec */
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
                boost::array<char, 8089> incomingData {}; // TODO fix: what if they send more than 8089 char at once?
                socket.receive(boost::asio::buffer(incomingData));
                std::string incomingDataCast(incomingData.begin(), incomingData.end());
                return static_cast<ProtocolDataPacket>(incomingDataCast);
            }
        }

    private:
        void _handleRequest([[maybe_unused]] std::list<boost::asio::ip::tcp::socket> &sockets,
                            boost::asio::ip::tcp::socket &socket,
                            const ProtocolDataPacket data,
                            std::function<ProtocolDataPacket(const ProtocolDataPacket &, int)> onPacketReceived)
        {
            const ProtocolDataPacket serverReplyToClient = onPacketReceived(data, socket.native_handle());
            if (serverReplyToClient.size()) // Works for std::string
                _sendMessage(socket, serverReplyToClient);
            socket.close();
            // TODO remove the socket from the list of sockets after closing it.
        }

        void _sendMessage(boost::asio::ip::tcp::socket & socket, const ProtocolDataPacket & message)
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
                socket.send(boost::asio::buffer(header));
                socket.send(boost::asio::buffer(message_serialized));
                //_socket->send_to(buffers, *_endpoints.begin()); // TODO : send it only once (merge header + message)
            }
            else if (_protocolType == TEXT) {
                auto messageCast = static_cast<std::string>(message);
                socket.send(boost::asio::buffer(messageCast));
            }
        }

    private:
        boost::asio::io_context                         _io_context;
        std::list<boost::asio::ip::tcp::socket>         _sockets;
        std::unique_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
        bool                _online;
        char                _inbound_header[8];
        size_t              _header_length;
        std::vector<char>   _inbound_data;
        ProtocolType        _protocolType;

    };
}