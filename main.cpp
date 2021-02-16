#include "Entite.hpp"

#include <fstream>

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
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        os << Ent{91} << Ent{13.37f} << Ent{"abc"};
    }

    /* LECTURE */
    {
        std::ifstream is(FILENAME, std::ifstream::binary);
        Ent ent{0};
        is >> ent; afficher(ent);
        is >> ent; afficher(ent);
        is >> ent; afficher(ent);
    }

    /* EVOLUTION */
    {
        Ent{123}.evolve();
    }
    
}