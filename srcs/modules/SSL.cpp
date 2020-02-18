/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSL.hpp"

zia::SSL::SSL()
        : _ctx { nullptr }
{
    _initOpenSSL();
    _createContext();
    _configureContext();
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
    const SSL_METHOD *method = SSLv23_server_method();

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
            oZ::State::BeforeParse, // Call at response creation time
            oZ::Priority::ASAP,
            this, &SSL::_onInterpret
    );
}

bool zia::SSL::_onInterpret(oZ::Context & context)
{
    std::cout << "SSL module has been called" << std::endl;

    ::SSL *ssl = SSL_new(_ctx);
    int fd = context.getPacket().getFileDescriptor();
    char reply[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!";

    SSL_set_fd(ssl, fd);
    std::cout << "after set fd" << std::endl;
    auto ret = SSL_accept(ssl);
    std::cout << "after ssl_accept : " << ret << std::endl;
    if (ret <= 0) {
        int err = SSL_get_error(ssl, ret);
        std::cerr << "err : " << err << std::endl;
        ERR_print_errors_fp(stderr);
        std::cerr << "[DEBUG ZIA] SSL Handshake failed" << std::endl;
        return false;
    }
    else {
        std::cout << "[DEBUG ZIA] SSL Handshake succeed ! OK" << std::endl;
        SSL_write(ssl, reply, static_cast<int>(strlen(reply)));
    }
    oZ::Log(oZ::Information) << "Module 'SSL' wrote successfully its message";
    context.getResponse().getReason() = "ssl";
    return true;
}

OPEN_ZIA_MAKE_ENTRY_POINT(zia::SSL)
