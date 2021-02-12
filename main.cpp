#include "Entite.hpp"

#include <fstream>
#include <vector>

void doIt(std::fstream& is)
{
    // while(is.peek() != std::ifstream::traits_type::eof())
    // {
        Entite<Forme> ent{0};
        is >> ent;
        ent.evolve();
    // }
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    const std::string FILENAME = "test";

    /* Enregistrer des entités pour effectuer le test */
    {
        std::fstream os(FILENAME, std::fstream::out | std::fstream::binary | std::fstream::trunc);
        os << Entite<Forme>{91} << Entite<Forme>{13.37f} << Entite<Forme>{"unechaine"}
           << Entite<Forme>{10} << Entite<Forme>{10.37f} << Entite<Forme>{"chaine10"}
           << Entite<Forme>{20} << Entite<Forme>{20.37f} << Entite<Forme>{"chaine20"}
           << Entite<Forme>{30} << Entite<Forme>{30.37f} << Entite<Forme>{"chaine30"};
    }
    
    /* Faire évoluer les entités */
    {
        // const unsigned NB_THREADS = std::thread::hardware_concurrency();
        const unsigned NB_THREADS = 2;
        std::fstream is(FILENAME, std::fstream::in | std::fstream::binary);
        std::vector<std::thread> pool;
        for(int i = 0; i < NB_THREADS; ++i)
            pool.push_back(std::thread(doIt, std::ref(is)));

        for(auto& t : pool)
            t.join();
    }
}