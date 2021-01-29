#include "Evoluable.h"

// g++ Evoluable.cpp main.cpp -std=c++17
int main()
{
    Evoluable* e = new Objet({91});
    // Evoluable* e = new Objet({91.f});
    // Evoluable* e = new Objet({""});

    e->afficher();
    e->evolve();

    // e->afficher();

    // e->evolve();
    // e->afficher();

    // e->evolve();
    // e->afficher();
    
    // e->evolve(); 


    delete(e);
}