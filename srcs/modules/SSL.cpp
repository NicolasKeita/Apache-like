/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#include <boost/asio/ssl.hpp>
#include "SSL.hpp"

zia::SSL::SSL()
        : _context { boost::asio::ssl::context::sslv23 }
{
    _context.set_verify_mode(boost::asio::ssl::verify_peer);
    //_context.load_verify_file("ca.pem");
}

const char * zia::SSL::getName() const noexcept
{
    return "SSL";
}


void zia::SSL::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::Interpret, // Call at response creation time
            oZ::Priority::ASAP,
            this, &SSL::_onInterpret
    );
}

bool zia::SSL::_onInterpret(oZ::Context &context)
{
    oZ::Log(oZ::Information) << "Module 'SSL' wrote successfully its message";
    std::cout << "SSL module has been called" << std::endl;

    (void)context;
    //exit(43);

    return true;
}

extern "C" oZ::IModule * CreateModule(void) { return new zia::SSL; }
