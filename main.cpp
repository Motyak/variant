#include "Entite.hpp"

#include <fstream>
#include <vector>

#define FILENAME "test"

void doIt()
{
    std::ifstream is(FILENAME, std::ifstream::binary);
    Entite<Forme> ent{0};

    while(is.peek() != std::ifstream::traits_type::eof())
    {
        is >> ent;
        // ent.evolve();
    }
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* Enregistrer des entités pour effectuer le test */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);

        os << Entite<Forme>{91} << Entite<Forme>{13.37f} << Entite<Forme>{"unechaine"}
           << Entite<Forme>{10} << Entite<Forme>{10.37f} << Entite<Forme>{"chaine10"}
           << Entite<Forme>{20} << Entite<Forme>{20.37f} << Entite<Forme>{"chaine20"}
           << Entite<Forme>{30} << Entite<Forme>{30.37f} << Entite<Forme>{"chaine30"};
    }
    
    /* Faire évoluer les entités */
    {
        // const unsigned POOL_SIZE = std::thread::hardware_concurrency();
        const unsigned POOL_SIZE = 1;
        std::vector<std::thread> pool;
        // for(int i = 0; i < POOL_SIZE; ++i)
        //     pool.push_back(std::thread(doIt));

        // for(auto& t : pool)
        //     t.join();



        std::ifstream is(FILENAME, std::ifstream::binary);
        Entite<Forme> ent{0};

        while(is.peek() != std::ifstream::traits_type::eof())
        {
            is >> ent;
            // ent.evolve();
        }
    }
}