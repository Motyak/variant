#include "Evoluable.h"

Objet::Objet(Forme formeInitiale) : 
        forme(formeInitiale){}

void Objet::evolve()
{
    std::cout<<"evolution..."<<std::endl;
    std::visit(overload {
        [this](int& d) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            forme = {91.2f};
            this->afficher(); //debug
            this->evolve();
        },
        [this](float& f) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            forme = {"quatre-vingt onze"};
            this->afficher(); //debug
            this->evolve();
        },
        // [](std::string& str) { 
        [this](std::string& str) { 
            this->afficher(); //debug
            std::cout<<"evolution terminée"<<std::endl;
        }
    }, this->forme);
}

void Objet::afficher()
{
    std::visit(overload {
        [this](int& d) { std::cout<<d<<std::endl; },
        [this](float& f) { std::cout<<f<<std::endl; },
        [this](std::string& str) { std::cout<<str<<std::endl; }
    }, this->forme);
}