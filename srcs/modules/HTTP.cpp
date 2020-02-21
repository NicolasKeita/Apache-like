/*
** Creation Year : 2020 ; Month : 02 ; Day : 20.
*/

#include "HTTP.hpp"

const char * zia::HTTP::getName() const noexcept
{
    return "HTTP";
}


void zia::HTTP::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::Interpret, // Call at response creation time
            oZ::Priority::Independent,
            this, &HTTP::_onInterpret
    );
}

bool zia::HTTP::_onInterpret(oZ::Context &context)
{
    if (context.getResponse().getReason() != "Interpret") {
        return true;
    }

    std::cout << "[DEBUG ZIA] HTTP Module called to create HTTP response" << std::endl;
    _createResponse(context);
//    context.setState(oZ::State::AfterInterpret);
    context.getResponse().getReason() = "AfterInterpret";
    return true;
}


void zia::HTTP::_createResponse(oZ::Context &context) const
{
    _createBodyToSend(context);
    _createHeaderToSend(context);
    std::cout << "[DEBUG ZIA] Response created in the main server" << std::endl;
}

void zia::HTTP::_createHeaderToSend(oZ::Context &context) const
{
    oZ::HTTP::Version version = context.getResponse().getVersion(); // TODO fix this unused operation
    version.majorVersion = 1;
    version.minorVersion = 1;

    oZ::HTTP::Code code = context.getResponse().getCode(); // TODO fix this unused operation
    code = oZ::HTTP::Code::OK;

    std::ostringstream versionToString;
    versionToString << "HTTP/" << std::to_string(version.majorVersion) << "." << std::to_string(version.minorVersion);
    context.getResponse().getHeader().set<std::string, std::string>("version",  versionToString.str()); //The fuck is this template

    context.getResponse().getHeader().set<std::string, std::string>("statut", "OK");
    context.getResponse().getHeader().set<std::string, std::string>("code", std::to_string(static_cast<double>(code)));
}

void zia::HTTP::_createBodyToSend(oZ::Context &context) const
{
    std::fstream fileStream("index.html", std::ios_base::in);
    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    fileStream.close();

//    std::cerr << "SIZE --- : " << sstr.str().size() << std::endl;
    context.getResponse().getHeader().set("content-type", "Content-Type: Multipart");
    auto contentSize = sstr.str().size();
    context.getResponse().getHeader().set("content-length", "Content-Length: " + std::to_string(contentSize));
    context.getResponse().getBody() = sstr.str();
}

OPEN_ZIA_MAKE_ENTRY_POINT(zia::HTTP)
