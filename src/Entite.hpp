#ifndef ENTITE_H
#define ENTITE_H

#include "Evoluable.hpp"

#include <cereal/types/string.hpp>

#include <thread>
#include <chrono>

struct Entite : public Evoluable, Evoluable::Forme<int,float,std::string>
{
    Entite() = default;
    Entite(const std::variant<int,float,std::string>& formeInitiale) : Evoluable::Forme<int,float,std::string>(formeInitiale){}

    template<class Archive>
    void serialize(Archive& ar) { ar(cereal::base_class<Evoluable::Forme<int,float,std::string>>(this)); }

    bool evoluer()
    {
        std::cout<<"evolution..."<<std::endl;
        return std::visit(Evolution {
            [&](int d) -> bool {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                *this = {d + 100.f};
                return true;
            },
            [&](float f) -> bool {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                *this = {"str" + std::to_string(f)};
                return true;
            },
            [](const std::string& str) -> bool {
                std::cout<<"evolution terminée : "<<str<<std::endl;
                return false;
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

    bool evoluer()
    {
        std::cout<<"evolution..."<<std::endl;
        return std::visit(Evolution {
            [&](double dou) -> bool {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                *this = {(char)((int)dou)};
                return true;
            },
            [&](char c) -> bool {
                std::cout<<"évolution terminée : "<<std::get<char>(forme)<<std::endl;
                return false;
            }
        }, this->forme);
    }
};
CEREAL_REGISTER_TYPE(Entite2);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Evoluable, Entite2);

#endif