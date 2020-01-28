//
// Created by Nicolas KEITA on 1/10/20.
//

#define BOOST_TEST_MODULE Test_ParserJsonWrapper.cpp
#include <boost/test/included/unit_test.hpp>
#include <boost/none.hpp>
#include "ParserJsonWrapper.hpp"
#include "nlohmann/json.hpp"

BOOST_AUTO_TEST_CASE( Test_ParserJsonWrapper_bad_filename )
{
    std::cerr << "[TEST libmy_parsers] Ignore test err log" << std::endl;
    BOOST_CHECK(ParserJsonWrapper::loadJson("bad_filename.json") == boost::none);
    std::cerr << "[TEST libmy_parsers] err log end" << std::endl;
}

BOOST_AUTO_TEST_CASE( Test_ParserJsonWrapper_correct_json )
{
    // Compare with a file .json
    nlohmann::json  expectedResult = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                {"everything", 42}
            }},
            {"list", {1, 0, 2}},
            {"object", {
                {"currency", "USD"},
                {"value", 42.99}
            }}
    };
    BOOST_CHECK(ParserJsonWrapper::loadJson("json_correct.json") == expectedResult);
}
