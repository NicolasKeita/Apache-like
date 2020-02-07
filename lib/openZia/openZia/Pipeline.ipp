/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Pipeline Abstraction
*/


#include <iostream>

template<typename ModuleType>
void oZ::Pipeline::registerCallback(State state, Priority priority, ModuleType *target, bool(ModuleType::*callback)(Context &))
{
    std::cout << "CALL 0.4" << std::endl;
    registerCallback(state, priority, [target, callback](Context &context) {
        std::cout << "CALL 1" << std::endl;
        return (target->*callback)(context);
    });
}

template<typename Type, typename ...Args>
void oZ::Pipeline::addModule(Args &&...args)
{
    _modules.emplace_back(std::make_shared<Type>(std::forward<Args>(args)...));
}

template<typename Type>
std::shared_ptr<Type> oZ::Pipeline::findModule(void) const
{
    for (auto &module : _modules)
        if (auto ptr = std::dynamic_pointer_cast<Type>(module); ptr)
            return ptr;
    return std::shared_ptr<Type>();
}