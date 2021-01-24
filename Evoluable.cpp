#include "Evoluable.h"

Objet::Objet(Forme formeInitiale) : 
        forme(formeInitiale){}

void Objet::evolve()
{
    std::cout<<"evolution..."<<std::endl;
    std::visit(overload {
        [this](int& d) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            forme = {91.f};
        },
        [this](float& f) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            forme = {"quatre-vingt onze"};
        },
        [this](std::string& str) { 
            std::cout<<"evolution terminée"<<std::endl;
        }
    }, this->forme);
}