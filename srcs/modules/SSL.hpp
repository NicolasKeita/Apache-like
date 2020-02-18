/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#pragma once

#include <openZia/IModule.hpp>
#include <openZia/ILogger.hpp>
#include <openZia/Log.hpp>
#include <openZia/Pipeline.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace zia {
    class SSL : public oZ::IModule {
    public:
        SSL();
        [[nodiscard]] const char *  getName() const noexcept override;
        void                        onRegisterCallbacks(oZ::Pipeline &pipeline) override;

    private:
        bool                        _onInterpret(oZ::Context &context);
        void static                 _initOpenSSL();
        void                        _createContext();
        void                        _configureContext();

    private:
        boost::asio::ssl::context   _context;
        SSL_CTX *                   _ctx;
    };
}

