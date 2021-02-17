#include "Entite.hpp"

#include <fstream>
#include <memory>

#define FILENAME "test"

//debug
void afficher(const Entite& ent)
{
    std::visit(overload {
        [](const int& d) { std::cout<<d<<std::endl; },
        [](const float& f) { std::cout<<f<<std::endl; },
        [](const std::string& str) { std::cout<<str<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ent.donnees);
}

//debug
void afficher(const Entite2& ent)
{
    std::visit(overload {
        [](const double& dou) { std::cout<<dou<<std::endl; },
        [](const char& c) { std::cout<<c<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ent.donnees);
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* ECRITURE */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        cereal::BinaryOutputArchive oarchive(os);
        std::shared_ptr<Evoluable> ev1 = std::make_shared<Entite>(13.37f);
        std::shared_ptr<Evoluable> ev2 = std::make_shared<Entite>("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc");
        std::shared_ptr<Evoluable> ev3 = std::make_shared<Entite2>(91.0);
        std::shared_ptr<Evoluable> ev4 = std::make_shared<Entite2>('x');
        oarchive(ev1, ev2, ev3, ev4);
    }

    /* LECTURE */
    {
        std::ifstream is(FILENAME, std::ifstream::binary);
        cereal::BinaryInputArchive iarchive(is);
        std::shared_ptr<Evoluable> ev1 = std::make_shared<Entite>(0);
        std::shared_ptr<Evoluable> ev2 = std::make_shared<Entite>(0);
        std::shared_ptr<Evoluable> ev3 = std::make_shared<Entite2>(0.0);
        std::shared_ptr<Evoluable> ev4 = std::make_shared<Entite2>(0.0);
        iarchive(ev1, ev2, ev3, ev4);
        afficher(*std::static_pointer_cast<Entite>(ev1).get());
        afficher(*std::static_pointer_cast<Entite>(ev2).get());
        afficher(*std::static_pointer_cast<Entite2>(ev3).get());
        afficher(*std::static_pointer_cast<Entite2>(ev4).get());
    }

    /* EVOLUTION */
    {
        Entite{123}.evolve();
        Entite2{65.1}.evolve();
    }
    
}