#include "Entite.hpp"

#include <fstream>

#define FILENAME "test"

/* debug */
void afficher(const Entite&);
void afficher(const Entite2&);

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* ECRITURE */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        for(const auto& e : std::vector<EvoluablePtr>{
            std::make_shared<Entite>(13.37f),
            std::make_shared<Entite>("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc"),
            std::make_shared<Entite2>(91.0),
            std::make_shared<Entite2>('x')
        })
            os << e;
    }

    /* LECTURE */
    {
        std::ifstream is(FILENAME, std::ifstream::binary);
        std::vector<EvoluablePtr> ev {
            std::make_shared<Entite>(),  std::make_shared<Entite>(),
            std::make_shared<Entite2>(), std::make_shared<Entite2>()
        };
        for(auto& e : ev)
            is >> e;

        /* debug */
        for(int i = 0 ; i < 2 ; ++i)
            afficher(*std::static_pointer_cast<Entite>(ev[i]).get());
        for(int i = 2 ; i < 4 ; ++i)
            afficher(*std::static_pointer_cast<Entite2>(ev[i]).get());
    }

    /* EVOLUTION */
    {
        Entite{123}.evolve();
        Entite2{65.1}.evolve();
    } 
}

/* debug */
void afficher(const Entite& ent)
{
    std::visit(overload {
        [](const int& d) { std::cout<<d<<std::endl; },
        [](const float& f) { std::cout<<f<<std::endl; },
        [](const std::string& str) { std::cout<<str<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ent.forme);
}
void afficher(const Entite2& ent)
{
    std::visit(overload {
        [](const double& dou) { std::cout<<dou<<std::endl; },
        [](const char& c) { std::cout<<c<<std::endl; },
        [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
    }, ent.forme);
}