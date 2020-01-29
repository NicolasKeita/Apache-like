/*
** EPITECH PROJET, 2018
** testMyStrTok.cpp
** File description:
** This function cut word into parts
*/

#include <criterion/criterion.h>
#include <vector>
#include "MyStrTok.hpp"

Test(myStrTok1, myStrTok1)
{
    std::vector<std::string> v;
    uti::myStrTok("hello world", v);
    cr_assert(v.at(0) == "hello");
    cr_assert(v.at(1) == "world");
}

Test(myStrTok2, myStrTok2)
{
    std::vector<std::string> v;
    uti::myStrTok("hello world  ", v);
    cr_assert(v.at(0) == "hello");
    cr_assert(v.at(1) == "world");
}

Test(myStrTok3, myStrTok3)
{
    std::vector<std::string> v;
    uti::myStrTok("   hello world  ", v);
    cr_assert(v.at(0) == "hello");
    cr_assert(v.at(1) == "world");
}

Test(myStrTok4, myStrTok4)
{
    std::vector<std::string> v;
    uti::myStrTok("   hello   world", v);
    cr_assert(v.at(0) == "hello");
    cr_assert(v.at(1) == "world");
}

Test(myStrTok5, myStrTok5)
{
    std::vector<std::string> v;
    uti::myStrTok("   hello   world, bye", v, " ,");
    cr_assert(v.at(0) == "hello");
    cr_assert(v.at(1) == "world");
    cr_assert(v.at(2) == "bye");
}

Test(myStrTok6, myStrTok6)
{
    std::vector<std::string> v;
    uti::myStrTok("   ", v, " ,");
    cr_assert(v.empty());
}
