#include "Evolueur.hpp"

#include <fstream>

#define FILENAME "test"

using Ev = Evoluable<int,float,std::string>;

//debug
void afficher(const Ev& ev)
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
    std::cout<<"ECRITURE"<<std::endl;
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        os << Ev{91} << Ev{13.37f} << Ev{"unechaineunechaineunechaineunechaineunechaineunechaine"};
    }

    std::cout<<std::endl<<"LECTURE"<<std::endl;
    {
        std::ifstream is(FILENAME, std::ifstream::binary);
        Ev ev{0};
        is >> ev; afficher(ev);
        is >> ev; afficher(ev);
        is >> ev; afficher(ev);
    }
    
}