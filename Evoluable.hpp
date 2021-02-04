#ifndef EVOLUABLE_H
#define EVOLUABLE_H

#include <thread>
#include <chrono>
#include <variant>
#include <iostream>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using Forme = std::variant<int,float,std::string>;

template<typename T>
struct Evoluable
{
    T forme;

    Evoluable(T formeInitiale) : forme(formeInitiale){}
    virtual void evolve() = 0;
    virtual void afficher() = 0;
};

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
                forme = {91.2f};
                this->afficher(); //debug
                this->evolve();
            },
            [this](const float& f) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                forme = {"quatre-vingt onze"};
                this->afficher(); //debug
                this->evolve();
            },
            // [](std::string& str) { 
            [this](const std::string& str) {  //debug
                this->afficher(); //debug
                std::cout<<"evolution terminée"<<std::endl;
            }
        }, this->forme);
    }

    void afficher()
    {
        std::visit(overload {
            [](const int& d) { std::cout<<d<<std::endl; },
            [](const float& f) { std::cout<<f<<std::endl; },
            [](const std::string& str) { std::cout<<str<<std::endl; }
        }, this->forme);
    }
};
#endif
