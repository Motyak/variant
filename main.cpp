#include "Entite.hpp"

#include <fstream>
#include <vector>
#include <mutex>

#define FILENAME "test"

void doIt(unsigned& index, std::mutex& mut)
{
    std::ifstream is(FILENAME, std::ifstream::binary);
    Entite<Forme> ent{0};

    mut.lock();
    is.seekg(sizeof(Forme) * index++, std::ios_base::beg);
    mut.unlock();

    while(is.peek() != std::ifstream::traits_type::eof())
    {
        std::cout<<index<<std::endl;
        is >> ent;
        // ent.evolve();

        mut.lock();
        is.seekg(sizeof(Forme) * index++, std::ios_base::beg);
        mut.unlock();
    }
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* Enregistrer des entités pour effectuer le test */
    // {
    //     std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);

    //     os << Entite<Forme>{91} << Entite<Forme>{13.37f} << Entite<Forme>{"unechaine"}
    //        << Entite<Forme>{10} << Entite<Forme>{10.37f} << Entite<Forme>{"chaine10"}
    //        << Entite<Forme>{20} << Entite<Forme>{20.37f} << Entite<Forme>{"chaine20"}
    //        << Entite<Forme>{30} << Entite<Forme>{30.37f} << Entite<Forme>{"chaine30"};
    // }
    
    /* Faire évoluer les entités */
    {
        // const unsigned POOL_SIZE = std::thread::hardware_concurrency();
        const unsigned POOL_SIZE = 1;
        
        unsigned index = 0u;
        std::mutex mut;
        std::vector<std::thread> pool;
        for(int i = 0; i < POOL_SIZE; ++i)
            pool.push_back(std::thread(doIt, std::ref(index), std::ref(mut)));

        for(auto& t : pool)
            t.join();
    }
}