/*
** Creation Year : 2020 ; Month : 01 ; Day : 30.
*/

#pragma once
#include <string>

struct ProtocolDataPacket {
    std::string message;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & message;
    }
};