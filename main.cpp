#include "Entite.hpp"

#include <fstream>
#include <memory>

#define FILENAME "test"

using Ent = Entite<int,float,std::string>;

//debug
void afficher(const Ent& ent)
{
    std::visit(overload {
        [](const int& d) { std::cout<<d<<std::endl; },
        [](const float& f) { std::cout<<f<<std::endl; },
        [](const std::string& str) { std::cout<<str<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ent.forme);
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* ECRITURE */
    {
        // std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        // cereal::BinaryOutputArchive oarchive(os);
        // std::shared_ptr<Evoluable<int,float,std::string>> ev = std::make_shared<Entite<int,float,std::string>>({91});
        // oarchive(ev);
    }

    /* LECTURE */
    {
        // std::ifstream is(FILENAME, std::ifstream::binary);
        // cereal::BinaryInputArchive iarchive(is);
        // Ent ent{0};
        // iarchive(ent);
        // afficher(ent);
    }

    /* EVOLUTION */
    // {
    //     Ent{123}.evolve();
    // }
    
}