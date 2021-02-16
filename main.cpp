#include "Evolueur.hpp"

#include <fstream>

#define FILENAME "test"

// g++ main.cpp -std=c++17 -pthread
int main()
{
    using Ev = Evoluable<int,float,std::string>;

    /* Ecriture */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        os << Ev{91} << Ev{13.37f} << Ev{"unechaine"};
    }

    /* Lecture */
    {
        // std::ifstream is(FILENAME, std::ifstream::binary);
        // Evoluable<int,float,std::string> ev{0};
        // is >> ent;

        // // affichage debug
        // std::visit(overload {
        //     [](const int& d) { std::cout<<d<<std::endl; },
        //     [](const float& f) { std::cout<<f<<std::endl; },
        //     [](const std::string& str) { std::cout<<str<<std::endl; },
        //     [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
        // }, ev);
    }
    
}