#ifndef ENTITE_H
#define ENTITE_H

#include "Evolueur.hpp"

#include "cereal/types/string.hpp"

#include <thread>
#include <chrono>

struct Entite : public Evoluable, Evoluable::Forme<int,float,std::string>
{
    Entite() = default;
    Entite(const std::variant<int,float,std::string>& formeInitiale) : Evoluable::Forme<int,float,std::string>(formeInitiale){}

    template<class Archive>
    void serialize(Archive& ar) { ar(cereal::base_class<Evoluable::Forme<int,float,std::string>>(this)); }

    void evolve()
    {
        std::cout<<"evolution..."<<std::endl;
        std::visit(overload {
            [this](const int& d) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                donnees = {d + 100.f};
                this->evolve();
            },
            [this](const float& f) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                donnees = {"str" + std::to_string(f)};
                this->evolve();
            },
            // [](std::string& str) { 
            [this](std::string& str) { //debug
                std::cout<<"evolution terminée : "<<std::get<std::string>(donnees)<<std::endl;
            }
        }, this->donnees);
    }
};

CEREAL_REGISTER_TYPE(Evoluable);
// CEREAL_REGISTER_TYPE(Evoluable::Forme<int,float,std::string>);
CEREAL_REGISTER_TYPE(Entite);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Evoluable, Entite);

#endif