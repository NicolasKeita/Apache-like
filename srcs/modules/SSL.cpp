/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSL.hpp"


zia::SSL::SSL()
        : _ctx { nullptr },
          _ssl_per_client { nullptr }
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
//    OpenSSL_add_ssl_algorithms();
    OpenSSL_add_all_algorithms();
    SSL_library_init();

}

const char * zia::SSL::getName() const noexcept
{
    return "SSL";
}

void zia::SSL::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::BeforeParse, // Call at accept client time
            oZ::Priority::ASAP,
            this, &SSL::_onBeforeParse
    );
    pipeline.registerCallback(
            oZ::State::AfterInterpret, // Call after response creation
            oZ::Priority::ASAP,
            this, &SSL::_onAfterInterpret
    );
}

bool zia::SSL::_onBeforeParse(oZ::Context & context)
{
    std::cout << "[DEBUG ZIA] SSL module has been called" << std::endl;

    int fd = context.getPacket().getFileDescriptor();
    bool ret = _handshake(fd);

    context.setState(oZ::State::Interpret);
    return ret;
}

bool zia::SSL::_onAfterInterpret(oZ::Context &context)
{
    char reply[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!";

    SSL_write(_ssl_per_client, reply, static_cast<int>(strlen(reply)));

    return true;
}

bool zia::SSL::_handshake(int fd_to_the_client)
{
    _ssl_per_client = SSL_new(_ctx); // TODO : free later

    SSL_set_fd(_ssl_per_client, fd_to_the_client);

    auto ret = SSL_accept(_ssl_per_client);

    if (ret <= 0) {
        int err = SSL_get_error(_ssl_per_client, ret);
        std::cerr << "[DEBUG ZIA] err detected during handshake SSL_accept returns : " << ret
                << " SSL_get_error returns : " << err << std::endl;
        ERR_print_errors_fp(stderr);
        return false;
    } else {
        std::cout << "[DEBUG ZIA] SSL Handshake succeed ! OK" << std::endl;
    }
    return true;
}

OPEN_ZIA_MAKE_ENTRY_POINT(zia::SSL)
