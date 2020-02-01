/*
** Creation Year : 2020 ; Month : 01 ; Day : 28.
*/

#include "my_prog_args/MyProgArgs.hpp"
#include "Server.hpp"
#include "ProtocolDataPacket.hpp"

unsigned short int extractPortNumberFromArgs(const std::string & argument1)
{
    unsigned short int port;
    try
    {
        port = static_cast<unsigned short int>(std::stoi(argument1));
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Wrong port : " << argument1
                << "\n" << e.what() << std::endl;
        exit(84);
    }
    return port;
}

int main(int argc, char *argv[], char **env)
{
    uti::MyProgArgs args(argc, argv, env, 1);

    unsigned short int port = extractPortNumberFromArgs(args.getArgs().at(1));
    zia::Server<zia::ProtocolDataPacket> server(port);
    return 0;
}