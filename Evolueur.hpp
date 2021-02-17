#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/variant.hpp"
#include "cereal/types/string.hpp"

#include <iostream>
#include <variant>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

template<typename... Types>
struct Evoluable
{
    std::variant<Types...> forme;

    Evoluable() : forme(std::variant<Types...>()){}//
    Evoluable(std::variant<Types...> formeInitiale) : forme(formeInitiale){}

    template<class Archive>
    void serialize(Archive& ar) { ar(forme); }

    virtual void evolve() = 0;
};

#endif