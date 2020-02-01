/*
** EPITECH PROJECT, 2019
** ClientUdpMultiThreadWrapper.hpp
** File description:
**
*/

#ifndef MY_NETWORK_LIBRARY_CLIENTUDPMULTITHREADWRAPPER_HPP
#define MY_NETWORK_LIBRARY_CLIENTUDPMULTITHREADWRAPPER_HPP

#include <memory>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "IClientUdpMultiThreadWrapper.hpp"

namespace uti::network {
    class ClientUdpMultiThreadWrapper : public IClientUdpMultiThreadWrapper {
        public:
            ClientUdpMultiThreadWrapper();

            void setServer(const std::string &serverAddress, unsigned short port) override;

            template<class T>
            void sendMessage(const T & message)
            {
                if (!_serverSet) {
                    std::cerr << "[Network ClientUdpMultiThread] Where to send ? You first have to set a host" << std::endl;
                    exit(30);
                }

                // Serialization
                std::ostringstream archive_stream;
                boost::archive::text_oarchive archive(archive_stream);
                archive << message;
                std::string message_serialized = archive_stream.str();

                // Header creation
                std::ostringstream  header_stream;
                header_stream << std::setw(_header_length) \
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
                _socket->send_to(boost::asio::buffer(header), *_endpoints.begin());
                _socket->send_to(boost::asio::buffer(message_serialized), *_endpoints.begin());
                //_socket->send_to(buffers, *_endpoints.begin()); // TODO : send it only once (merge header + message)
            }

            template<class T>
            T getReply()
            {
                //using boost::asio::ip::udp;

                // Receive the header
                _socket->receive(boost::asio::buffer(inbound_header));

                std::istringstream is(std::string(inbound_header, _header_length));
                std::size_t inbound_data_size = 0;

                if (!(is >> std::hex >> inbound_data_size)) {
                    std::cerr << "[CLientUdpMultiThread] Header is not valid : " << std::dec << inbound_data_size << std::endl;
                    exit(31);
                }
                // Conversion hex to dec
                std::stringstream stream;
                size_t inbound_data_size_in_decimal;
                stream << inbound_data_size;
                stream >> std::hex >> inbound_data_size_in_decimal;

                inbound_data.resize(inbound_data_size_in_decimal);

                _socket->receive(boost::asio::buffer(inbound_data));

                T t;
                try {
                    std::string archive_data(&inbound_data[0], inbound_data.size());
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> t;
                }
                catch (std::exception & e)
                {
                    std::cerr << "[CLientUdpMultiThread] Unable to decode data." << std::endl;
                    exit(34);
                }
                return t;
            }

            void stop() override;

        private:
            boost::asio::io_context                         _io_context;
            std::unique_ptr<boost::asio::ip::udp::socket>   _socket;
            boost::asio::ip::udp::resolver                  _resolver;
            boost::asio::ip::udp::resolver::results_type    _endpoints;
            std::string             _serverAddress;
            size_t                  _port;
            bool                    _serverSet;
            size_t                  _header_length;
            char                    inbound_header[8];
            std::vector<char>       inbound_data;
    };
}

#endif //MY_NETWORK_LIBRARY_CLIENTUDPMULTITHREADWRAPPER_HPP
