//
// Created by Nicolas KEITA on 2/7/20.
//

#include "Hello.hpp"

extern "C" oZ::IModule * CreateModule(void) { return new oZ::Hello; }
