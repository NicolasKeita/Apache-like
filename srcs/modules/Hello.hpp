//
// Created by Nicolas KEITA on 2/7/20.
//

/* --- Hello.hpp --- */
#pragma once

#include <openZia/IModule.hpp>
#include <openZia/ILogger.hpp>
#include <openZia/Log.hpp>
#include <openZia/Pipeline.hpp>

namespace oZ {
    class Hello : public oZ::IModule {
    public:
        Hello(void) = default;

        virtual ~Hello(void) = default;

        virtual const char *getName(void) const { return "Hello"; }

        virtual Dependencies getDependencies(void) const noexcept { return {}; }

        // Register your callback to the pipeline
        virtual void onRegisterCallbacks(oZ::Pipeline &pipeline) {
            pipeline.registerCallback(
                    oZ::State::Interpret, // Call at response creation time
                    static_cast<oZ::Priority>(oZ::Priority::Medium + 1), // With medium priority but higher than 'World' module
                    this, &Hello::onInterpret // Member function style
            );
        }

    private:
        bool onInterpret(oZ::Context &context) {
            oZ::Log(oZ::Information) << "Module 'Hello' wrote successfully its message";
            context.getResponse().getHeader().get("Content-Type") = "text/plain";
            context.getResponse().getBody() += "Hello";
            return true;
        }
    };
}