#ifndef ENTITE_H
#define ENTITE_H

#include "Evolueur.hpp"

#include <thread>
#include <chrono>

// using Forme = std::variant<int,float,std::string>;

template<typename... Types>
struct Entite : public Evoluable<Types...>{};

template<>
struct Entite<int,float,std::string> : public Evoluable<int,float,std::string>
{
    Entite(std::variant<int,float,std::string> formeInitiale) : Evoluable(formeInitiale){}

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