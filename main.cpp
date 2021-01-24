#include "Evoluable.h"

// g++ Evoluable.cpp main.cpp
int main()
{
    Evoluable* e = new Objet({91});
    // Evoluable* e = new Objet({91.f});
    // Evoluable* e = new Objet({""});

    e->evolve();
    e->evolve();
    e->evolve();
    

    delete(e);
}