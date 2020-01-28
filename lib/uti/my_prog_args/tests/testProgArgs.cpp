/*
** EPITECH PROJET, 2018
** testMyStrTok.cpp
** File description:
** This function cut word into parts
*/

#include <criterion/criterion.h>
#include "MyProgArgs.hpp"

Test(myProgArgs, myProgArgs)
{
    int     argc = 4;
    char    **argv = new char *[5];
    if (argv == nullptr)
        exit(84);
    argv[0] = strdup("My");
    argv[1] = strdup("name");
    argv[2] = strdup("is");
    argv[3] = strdup("Nicolas");
    argv[4] = nullptr;
    uti::MyProgArgs progArgs(argc, argv);
    cr_assert(progArgs.getSize() == 4);
    cr_assert(progArgs.getArgs().at(0) == "My");
    cr_assert(progArgs.getArgs().back() == "Nicolas");
}

