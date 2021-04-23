#include "Entite.hpp"
#include "Evolueur.hpp"

#include <csignal>

// g++ -std=c++17 src/main.cpp -o evolueur -lredis++ -lhiredis -lstdc++fs -pthread 
int main()
{
    /* ECRITURE */
    {
        auto redis = sw::redis::Redis("tcp://" + Evolueur::HOST + ":" + Evolueur::PORT);
        for(const auto& e : std::vector<EvoluablePtr>{
            std::make_shared<Entite>(91), std::make_shared<Entite>(13.37f),
            std::make_shared<Entite>("unechaine,unechaine,unechaine"),
            std::make_shared<Entite>(10), std::make_shared<Entite>(10.37f),
            std::make_shared<Entite>("unechaine10,unechaine10,unechaine10"),
            std::make_shared<Entite2>(84.0), std::make_shared<Entite2>('a'),
            std::make_shared<Entite2>(79.0), std::make_shared<Entite2>('b'),
            std::make_shared<Entite2>(77.0), std::make_shared<Entite2>('c'),
            std::make_shared<Entite>(910), std::make_shared<Entite>(130.37f),
            std::make_shared<Entite>("_unechaine,unechaine,unechaine"),
            std::make_shared<Entite>(100), std::make_shared<Entite>(100.37f),
            std::make_shared<Entite>("_unechaine10,unechaine10,unechaine10"),
            std::make_shared<Entite2>(840.0), std::make_shared<Entite2>('A'),
            std::make_shared<Entite2>(790.0), std::make_shared<Entite2>('B'),
            std::make_shared<Entite2>(770.0), std::make_shared<Entite2>('C')
        })
            redis << e;       
    }

    /* EVOLUTION */
    {
        Evolueur e;
        signal(SIGINT, Evolueur::sigintHandler);
        e();
    }
}