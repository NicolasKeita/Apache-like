/*
** Creation Year : 2020 ; Month : 02 ; Day : 20.
*/

#pragma once

#include <string>
#include <utility>
#include <sstream>
#include <fstream>
#include <openZia/IModule.hpp>
#include <openZia/ILogger.hpp>
#include <openZia/Log.hpp>
#include <openZia/Pipeline.hpp>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace zia {
    class HTTP : public oZ::IModule {
    public:
        [[nodiscard]] const char *  getName() const noexcept override;
        void                        onRegisterCallbacks(oZ::Pipeline &pipeline) override;

    private:
        bool                        _onInterpret(oZ::Context &context);
        void                        _createResponse(oZ::Context &context) const;
        void                        _createHeaderToSend(oZ::Context &context) const;
        void                        _createBodyToSend(oZ::Context &context) const;
    };
}


