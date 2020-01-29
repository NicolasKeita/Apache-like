/*
** Creation Year : 2020 ; Month : 01 ; Day : 28.
*/

#include "my_prog_args/MyProgArgs.hpp"
#include "Server.hpp"

short int extractPortNumberFromArgs(const std::string & argument1)
{
    short int port;
    try
    {
        port = static_cast<short int>(std::stoi(argument1));
        if (port < 1)
            throw std::invalid_argument("[ZIA] Ports cannot be negative");
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

    short int port = extractPortNumberFromArgs(args.getArgs().at(1));
    zia::Server server(port);
    return 0;
}