/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSL.hpp"

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
typedef ssl::stream<tcp::socket> ssl_socket;

zia::SSL::SSL()
        : _context { boost::asio::ssl::context::sslv23 }
{
    _initOpenSSL();
    _createContext();
    _configureContext();
    _context.set_verify_mode(boost::asio::ssl::verify_peer);
    _context.load_verify_file("mycert.pem");
}

void zia::SSL::_configureContext()
{
    SSL_CTX_set_ecdh_auto(_ctx, 1);
    if (SSL_CTX_use_certificate_file(_ctx,
                                     "mycert.pem",
                                     SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(_ctx, "key.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "[DEBUG ZIA] Certificate and Key SSL are loaded !" << std::endl;
    }
}

void zia::SSL::_createContext()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    _ctx = SSL_CTX_new(method);
    if (!_ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void zia::SSL::_initOpenSSL()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
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

OPEN_ZIA_MAKE_ENTRY_POINT(zia::SSL)
