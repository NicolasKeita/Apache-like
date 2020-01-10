//
// Created by Nicolas KEITA on 1/10/20.
//

#include <iostream>
#include <fstream>
#include <string>
#include <boost/optional.hpp>
#include "ParserJsonWrapper.hpp"

boost::optional<nlohmann::json> ParserJsonWrapper::loadJson(const std::string &filename) const noexcept
{
    std::ifstream   i(filename);
    nlohmann::json  j;

    if (i) {
        i >> j;
        return j;
    } else {
        std::cerr << "Unable to read the file :  " << filename << std::endl;
        return boost::none;
    }
}