/*
** EPITECH PROJECT, 2019
** main.cpp
** File description:
**
*/

#include <iostream>
#include "Async/ServerTcpAsyncWrapper.hpp"

std::string handleMessageReceived(const std::string &messageReceived)
{
    std::cerr << "[Message received] : " << messageReceived << "FIN" << std::endl;
    if (messageReceived == "AB")
        return "CD";
    if (messageReceived == "EF")
        return "GH";
    if (messageReceived == "IJ")
        return "FIRST TEST SUCCESSFULLY COMPLETED !";
    if (messageReceived == "too")
        return "fast";
    if (messageReceived == "for you")
        return "SECOND TEST SUCCESFULLY COMPLETED !";
	return "";
}

int main()
{
    try
    {
        // Blocking
        
	uti::network::ServerTcpAsyncWrapper server;

        server.turnOn(42424,
			&handleMessageReceived,
			"WELCOME, you have successfully connected");
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

