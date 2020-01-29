/*
** Creation Year : 2020 ; Month : 01 ; Day : 28.
*/

#include "my_prog_args/MyProgArgs.hpp"
#include "Server.hpp"

int main(int argc, char *argv[], char **env)
{
    uti::MyProgArgs args(argc, argv, env, 0);

    zia::Server server;
    return 0;
}