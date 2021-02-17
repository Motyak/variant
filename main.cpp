#include "Entite.hpp"

#include <fstream>
#include <memory>

#define FILENAME "test"

using Ent = Entite<int,float,std::string>;

//debug
void afficher(const Evoluable<int,float,std::string>& ev)
{
    std::visit(overload {
        [](const int& d) { std::cout<<d<<std::endl; },
        [](const float& f) { std::cout<<f<<std::endl; },
        [](const std::string& str) { std::cout<<str<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ev.forme);
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* ECRITURE */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        cereal::BinaryOutputArchive oarchive(os);
        std::shared_ptr<Evoluable<int,float,std::string>> ev1 = std::make_shared<Entite<int,float,std::string>>(91);
        std::shared_ptr<Evoluable<int,float,std::string>> ev2 = std::make_shared<Entite<int,float,std::string>>(13.37f);
        std::shared_ptr<Evoluable<int,float,std::string>> ev3 = std::make_shared<Entite<int,float,std::string>>("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc");
        oarchive(ev1, ev2, ev3);
    }

    /* LECTURE */
    {
        std::ifstream is(FILENAME, std::ifstream::binary);
        cereal::BinaryInputArchive iarchive(is);
        std::shared_ptr<Evoluable<int,float,std::string>> ev1 = std::make_shared<Entite<int,float,std::string>>(0);
        std::shared_ptr<Evoluable<int,float,std::string>> ev2 = std::make_shared<Entite<int,float,std::string>>(0);
        std::shared_ptr<Evoluable<int,float,std::string>> ev3 = std::make_shared<Entite<int,float,std::string>>(0);
        iarchive(ev1, ev2, ev3);
        afficher(*ev1.get());
        afficher(*ev2.get());
        afficher(*ev3.get());
    }

    /* EVOLUTION */
    // {
    //     Ent{123}.evolve();
    // }
    
}