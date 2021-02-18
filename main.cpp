#include "Entite.hpp"

#include <fstream>
#include <thread>
#include <mutex>

#define FILENAME "test"

void doIt(std::istream& is, std::mutex& mut)
{
    EvoluablePtr ev;

    while(true)
    {
        /* extraction du prochain élément */
        mut.lock();
        if(is.peek() == std::ifstream::traits_type::eof())
        {
            mut.unlock();
            return; // ou 'continue' si on veut que ça continue de tourner
        }
        is >> ev;
        mut.unlock();

        /* operation sur l'element */
        ev->evolve();
    }
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* ECRITURE */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
        for(const auto& e : std::vector<EvoluablePtr>{
            std::make_shared<Entite>(91), std::make_shared<Entite>(13.37f),
            std::make_shared<Entite>("unechaine,unechaine,unechaine"),
            std::make_shared<Entite>(10), std::make_shared<Entite>(10.37f),
            std::make_shared<Entite>("unechaine10,unechaine10,unechaine10"),
            std::make_shared<Entite2>(84.0), std::make_shared<Entite2>('a'),
            std::make_shared<Entite2>(79.0), std::make_shared<Entite2>('b'),
            std::make_shared<Entite2>(77.0), std::make_shared<Entite2>('c'),
        })
            os << e;
    }

    /* EVOLUTION */
    {
        const unsigned POOL_SIZE = std::thread::hardware_concurrency();
        std::ifstream is(FILENAME, std::ifstream::binary);
        std::mutex mut;
        std::vector<std::thread> pool;
        for(int i = 0; i < POOL_SIZE; ++i)
            pool.push_back(std::thread(doIt, std::ref(is), std::ref(mut)));

        for(auto& t : pool)
            t.join();
    }
}