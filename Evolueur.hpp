#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/variant.hpp"

#include <iostream>
#include <variant>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;


struct Evoluable
{
    virtual void evolve() = 0;

    template<typename... Types>
    struct Forme
    {
        std::variant<Types...> forme;

        Forme() = default;
        Forme(const std::variant<Types...>& formeInitiale) : forme(formeInitiale){}

        template<class Archive>
        void serialize(Archive& ar) { ar(forme); }
    };
};
CEREAL_REGISTER_TYPE(Evoluable);

#endif