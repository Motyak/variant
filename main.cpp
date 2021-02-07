#include "Evoluable.hpp"

#include <fstream>

void afficher(const Entite<Forme>& ent)
{
    std::visit(overload {
        [](const int& d) { std::cout<<d<<std::endl; },
        [](const float& f) { std::cout<<f<<std::endl; },
        [](const std::string& str) { std::cout<<str<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ent.forme);
}

// g++ main.cpp -std=c++17
int main()
{
    // Fait évoluer une entité ayant pour forme initiale 91 (int)
    // Entite<Forme>{91}.evolve();

    /* insert into file */
    {
        Entite<Forme> e1{91};
        Entite<Forme> e2{13.37f};
        Entite<Forme> e3{"unechaine"};

        std::fstream os("test", std::fstream::out | std::fstream::binary | std::fstream::trunc);
        os << e1 << e2 << e3;
    }

    /* extract from file */
    {
        Entite<Forme> e{"valeurpardefaut"};
        std::fstream is("test", std::fstream::in | std::fstream::binary);
        for(int i = 1 ; i <= 3 ; ++i)
        {
            is >> e;
            afficher(e);
        }
    }
}