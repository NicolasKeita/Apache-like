/*
** Creation Year : 2020 ; Month : 02 ; Day : 20.
*/

#include "HTTP.hpp"

const char * zia::HTTP::getName() const noexcept
{
    return "HTTP";
}


void zia::HTTP::onRegisterCallbacks(oZ::Pipeline &pipeline)
{

}

OPEN_ZIA_MAKE_ENTRY_POINT(zia::HTTP)
