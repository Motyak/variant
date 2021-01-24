#ifndef EVOLUABLE_H
#define EVOLUABLE_H

#include <thread>
#include <chrono>
#include <variant>
#include <iostream>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using Forme = std::variant<int,float,std::string>;

struct Evoluable
{
    virtual void evolve() = 0;
    virtual ~Evoluable() = default;
};

struct Objet : public Evoluable
{
    Forme forme;

    Objet(Forme formeInitiale);
    void evolve() override;
};
#endif