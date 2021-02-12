#ifndef ENTITE_H
#define ENTITE_H

#include "Evolueur.hpp"

#include <thread>
#include <chrono>
#include <variant>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using Forme = std::variant<int,float,std::string>;

template<typename T>
struct Entite : public Evoluable<T>{};

template<>
struct Entite<Forme> : public Evoluable<Forme>
{
    Entite(Forme formeInitiale) : Evoluable(formeInitiale){}

    void evolve()
    {
        std::cout<<"evolution..."<<std::endl;
        std::visit(overload {
            [this](const int& d) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                forme = {d + 100.f};
                this->evolve();
            },
            [this](const float& f) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                forme = {"str" + std::to_string(f)};
                this->evolve();
            },
            // [](std::string& str) { 
            [this](std::string& str) { //debug
                std::cout<<"evolution terminée : "<<std::get<std::string>(forme)<<std::endl;
            }
        }, this->forme);
    }
};

#endif