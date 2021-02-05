#include "Evoluable.hpp"

#include <fstream>

// g++ main.cpp -std=c++17
int main()
{
    // Fait évoluer une entité ayant pour forme initiale 91 (int)
    // Entite<Forme>{91}.evolve();

    // insert into file
    {
        Entite<Forme> e{91};
        std::fstream os("test", std::fstream::out | std::fstream::binary | std::fstream::trunc);
        os << e;
    }

    // extract from file (ne marche pas)
    {
        Entite<Forme> e{91};
        std::fstream is("test", std::fstream::in | std::fstream::binary);
        is >> e;
        std::visit(overload {
            [](const int& d) { std::cout<<d<<std::endl; },
            [](const float& f) { std::cout<<f<<std::endl; },
            [](const std::string& str) { std::cout<<str<<std::endl; },
            [](auto& onsaitpas) { std::cout<<"on sait pas"<<std::endl; }
        }, e.forme);
    }
}