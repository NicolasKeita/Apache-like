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
        [[nodiscard]] const char *getName() const noexcept override { return "SSL"; }
        void onRegisterCallbacks(oZ::Pipeline &pipeline) override {
            pipeline.registerCallback(
                    oZ::State::Interpret, // Call at response creation time
                    oZ::Priority::ASAP,
                    this, &SSL::onInterpret // Member function style
            );
        }

    private:
        bool onInterpret(oZ::Context &context) {
            oZ::Log(oZ::Information) << "Module 'SSL' wrote successfully its message";
            std::cout << "LOADING COMPLETE" << std::endl;
            context.getResponse().getHeader().get("Content-Type") = "text/plain";
            context.getResponse().getBody() += "Hello";
            return true;
        }

    };
}

