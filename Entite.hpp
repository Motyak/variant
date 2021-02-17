#ifndef ENTITE_H
#define ENTITE_H

#include "Evolueur.hpp"

#include <thread>
#include <chrono>

template<typename... Types>
struct Entite : public Evoluable<Types...>{};

template<>
struct Entite<int,float,std::string> : public Evoluable<int,float,std::string>
{
    Entite() : Evoluable(){}//
    Entite(const std::variant<int,float,std::string>& formeInitiale) : Evoluable(formeInitiale){}

    template<class Archive>
    void serialize(Archive& ar) { ar(cereal::base_class<Evoluable<int,float,std::string>>(this)); }

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

CEREAL_REGISTER_TYPE(Evoluable<int,float,std::string>);
CEREAL_REGISTER_TYPE(Entite<int,float,std::string>);

#endif