#include "Entite.hpp"

#include <fstream>
#include <vector>
#include <mutex>

#define FILENAME "test"

void doIt(std::istream& is, std::mutex& mut)
{
    Entite<Forme> ent{0};

    while(true)
    {
        /* extraction du prochain élément */
        mut.lock();
        if(is.peek() == std::ifstream::traits_type::eof())
        {
            mut.unlock();
            return;
        }
        is >> ent;
        mut.unlock();

        /* operation sur l'element */
        // ent.evolve();
    }
}

// g++ main.cpp -std=c++17 -pthread
int main()
{
    /* Enregistrer des entités pour effectuer le test */
    {
        std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);

        os << Entite<Forme>{91};
        os << Entite<Forme>{"123456789123456789123456789123456789"};
        os << Entite<Forme>{13.37f};

        // os << Entite<Forme>{91} << Entite<Forme>{13.37f} << Entite<Forme>{"unechaine"}
        //    << Entite<Forme>{10} << Entite<Forme>{10.37f} << Entite<Forme>{"chaine10"}
        //    << Entite<Forme>{20} << Entite<Forme>{20.37f} << Entite<Forme>{"chaine20"}
        //    << Entite<Forme>{30} << Entite<Forme>{30.37f} << Entite<Forme>{"chaine30"};
    }
    
    /* LECTURE DU FICHIER SEQUENTIELLE */
    {
        std::ifstream is(FILENAME, std::ifstream::binary);
        Entite<Forme> ent{0};
        is >> ent; afficher(ent.forme);
        is >> ent; afficher(ent.forme);
        is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);
        // is >> ent; afficher(ent.forme);////////////////////////////


        // while(is.peek() != std::ifstream::traits_type::eof())
        // {
        //     is >> ent;
        //     // ent.evolve();
        // }
    }

    /* Faire évoluer les entités */
    // {
    //     // const unsigned POOL_SIZE = std::thread::hardware_concurrency();
    //     const unsigned POOL_SIZE = 1;
    //     std::ifstream is(FILENAME, std::ifstream::binary);
    //     std::mutex mut;
    //     std::vector<std::thread> pool;
    //     for(int i = 0; i < POOL_SIZE; ++i)
    //         pool.push_back(std::thread(doIt, std::ref(is), std::ref(mut)));

    //     for(auto& t : pool)
    //         t.join();
    // }
}