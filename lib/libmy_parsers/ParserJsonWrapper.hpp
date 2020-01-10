//
// Created by Nicolas KEITA on 1/10/20.
//

#ifndef CPP_ZIA_2019_PARSERJSONWRAPPER_HPP
#define CPP_ZIA_2019_PARSERJSONWRAPPER_HPP

#include <boost/optional.hpp>
#include <string>
#include "nlohmann/json.hpp"

class ParserJsonWrapper {
    public:
        [[nodiscard]] auto loadJson(const std::string & filename) const noexcept -> boost::optional<nlohmann::json>;
};


#endif //CPP_ZIA_2019_PARSERJSONWRAPPER_HPP
