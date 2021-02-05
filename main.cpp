#include "Evoluable.hpp"

// g++ main.cpp -std=c++17
int main()
{
    // Fait évoluer une entité ayant pour forme initiale 91 (int)
    // Entite<Forme>{91}.evolve();

    Entite<Forme> e{13.37f};
    std::cout<<e<<std::endl;
}