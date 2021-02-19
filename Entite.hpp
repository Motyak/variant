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

    void evolve(const std::function<void (std::ostream&,std::mutex&)>& pre_trans, std::ostream& os, std::mutex& write)
    {
        std::cout<<"evolution..."<<std::endl;
        pre_trans(os, write);
        std::visit(Evolution {
            [&](const int& d) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                *this = {d + 100.f};
                evolve(pre_trans, os, write);
            },
            [&](const float& f) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                *this = {"str" + std::to_string(f)};
                evolve(pre_trans, os, write);
            },
            [&](std::string& str) {
                std::cout<<"evolution terminée : "<<std::get<std::string>(forme)<<std::endl;
            }
        }, this->forme);
    }
};
CEREAL_REGISTER_TYPE(Entite);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Evoluable, Entite);

struct Entite2 : public Evoluable, Evoluable::Forme<double,char>
{
    Entite2() = default;
    Entite2(const std::variant<double,char>& formeInitiale) : Evoluable::Forme<double,char>(formeInitiale){}

    template<class Archive>
    void serialize(Archive& ar) { ar(cereal::base_class<Evoluable::Forme<double,char>>(this)); }

    void evolve(const std::function<void (std::ostream&,std::mutex&)>& pre_trans, std::ostream& os, std::mutex& write)
    {
        std::cout<<"evolution..."<<std::endl;
        pre_trans(os, write);
        std::visit(Evolution {
            [&](const double& dou) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                *this = {(char)((int)dou)};
                evolve(pre_trans, os, write);
            },
            [&](const char& c) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout<<"évolution terminée : "<<std::get<char>(forme)<<std::endl;
            }
        }, this->forme);
    }
};
CEREAL_REGISTER_TYPE(Entite2);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Evoluable, Entite2);

#endif