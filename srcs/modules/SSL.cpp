/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSL.hpp"


int password_cb(char *buf, int size, int rwflag, void *password);

EVP_PKEY *generatePrivateKey();
X509 *generateCertificate(EVP_PKEY *pkey);

zia::SSL::SSL()
        : _ctx { nullptr }
{
    _initOpenSSL();
    _createContext();
    _configureContext();
}

void zia::SSL::_configureContext()
{
    EVP_PKEY *pkey = generatePrivateKey();
    X509 *x509 = generateCertificate(pkey);

    SSL_CTX_use_certificate(_ctx, x509);
    SSL_CTX_set_default_passwd_cb(_ctx, password_cb);
    SSL_CTX_use_PrivateKey(_ctx, pkey);

    RSA *rsa=RSA_generate_key(512, RSA_F4, NULL, NULL);
    SSL_CTX_set_tmp_rsa(_ctx, rsa);
    RSA_free(rsa);

    SSL_CTX_set_verify(_ctx, SSL_VERIFY_NONE, 0);

    /*
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
    }*/
    std::cout << "[DEBUG ZIA] Certificate and Key SSL are loaded !" << std::endl;
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

int password_cb(char *buf, int size, int rwflag, void *password);

EVP_PKEY *generatePrivateKey();
X509 *generateCertificate(EVP_PKEY *pkey);

int password_cb(char *buf, int size, int rwflag, void *password)
{
    strncpy(buf, (char *)(password), size);
    buf[size - 1] = '\0';
    return strlen(buf);
}

EVP_PKEY *generatePrivateKey()
{
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY_keygen_init(pctx);
    EVP_PKEY_CTX_set_rsa_keygen_bits(pctx, 2048);
    EVP_PKEY_keygen(pctx, &pkey);
    return pkey;
}

X509 *generateCertificate(EVP_PKEY *pkey)
{
    X509 *x509 = X509_new();
    X509_set_version(x509, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 0);
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), (long)60*60*24*365);
    X509_set_pubkey(x509, pkey);

    X509_NAME *name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (const unsigned char*)"US", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (const unsigned char*)"YourCN", -1, -1, 0);
    X509_set_issuer_name(x509, name);
    X509_sign(x509, pkey, EVP_md5());
    return x509;
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

    BIO *accept_bio = BIO_new_socket(fd, BIO_CLOSE);
    SSL_set_bio(ssl, accept_bio, accept_bio);

    std::cout << "after set fd" << std::endl;
    auto ret = SSL_accept(ssl);
    std::cerr << "after ssl_accept : " << ret << std::endl;

    BIO *bio = BIO_pop(accept_bio);

    if (ret <= 0) {
        int err = SSL_get_error(ssl, ret);
        std::cerr << "err : " << err << std::endl;
        ERR_print_errors_fp(stderr);
        std::cerr << "[DEBUG ZIA] SSL Handshake failed" << std::endl;
        return false;
    }
    std::cout << "[DEBUG ZIA] SSL Handshake succeed ! OK" << std::endl;
    SSL_write(ssl, reply, static_cast<int>(strlen(reply)));
    oZ::Log(oZ::Information) << "Module 'SSL' wrote successfully its message";
    context.getResponse().getReason() = "ssl";
    return true;
}

OPEN_ZIA_MAKE_ENTRY_POINT(zia::SSL)
