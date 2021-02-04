#include "Evoluable.h"

// g++ main.cpp -std=c++17
int main()
{
    // Fait évoluer une entité ayant pour forme initiale 91 (int)
    Entite<Forme>{91}.evolve();
}