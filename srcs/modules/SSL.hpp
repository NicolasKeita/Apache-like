/*
** Creation Year : 2020 ; Month : 02 ; Day : 10.
*/

#pragma once

#include <openZia/IModule.hpp>
#include <openZia/ILogger.hpp>
#include <openZia/Log.hpp>
#include <openZia/Pipeline.hpp>

namespace zia {
    class SSL : public oZ::IModule {
    public:
        SSL();
        [[nodiscard]] const char *  getName() const noexcept override;
        void                        onRegisterCallbacks(oZ::Pipeline &pipeline) override;

    private:
        bool                        _onInterpret(oZ::Context &context);

    private:
        boost::asio::ssl::context   _context;
        //boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
    };
}

