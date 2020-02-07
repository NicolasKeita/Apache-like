//
// Created by Nicolas KEITA on 2/7/20.
//

#include "Hello.hpp"

extern "C" oZ::ModulePtr CreateModule(void) { return std::make_shared<oZ::Hello>(); }

