/*
** Creation Year : 2020 ; Month : 02 ; Day : 20.
*/

#pragma once

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
    };
}


