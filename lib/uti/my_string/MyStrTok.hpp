/*
** EPITECH PROJET, 2018
** myStrTok.hpp
** File description:
** This function cuts a string into parts
*/

#ifndef MYSTRTOK_HPP
#define MYSTRTOK_HPP

#include <algorithm>
#include <string>

/*
 * See tests for better undestanding
 *
 * If delim is more than one character,
 * the function will split the string based on EACH character inside delim
 */
namespace uti {
    template<class Container>
    void myStrTok(const std::string &str, Container &cont,
                  const std::string &delims = " ") {
        if (str.empty())
            return;
        auto itBeginToken = str.begin();
        auto itEndToken = str.begin();
        cont.clear();

        while (itEndToken != str.end()) {
            std::size_t indexBeginToken =
                    str.find_first_not_of(delims,
                                          static_cast<unsigned long>(itEndToken - str.begin()));
            itBeginToken = str.begin() + static_cast<long>(indexBeginToken);
            if (itBeginToken == str.end() || indexBeginToken == std::string::npos)
                break;
            itEndToken = std::find_first_of(itBeginToken, str.end(),
                                            delims.begin(), delims.end());
            cont.push_back(std::string(itBeginToken, itEndToken));
        }
    }
}

#endif //MYSTRTOK_HPP
