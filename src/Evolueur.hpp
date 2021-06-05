#ifndef EVOLUEUR_HPP
#define EVOLUEUR_HPP

#include "Evoluable.hpp"
#include "Redis.hpp"

#include <thread>
#include <experimental/filesystem>
#include <fstream>

class Evolueur
{
    inline static const int POOL_SIZE = std::thread::hardware_concurrency(); 
    inline static const std::experimental::filesystem::path TEMP_DIR = std::experimental::filesystem::temp_directory_path() / "evolueur";

    void callbackFn(int threadId)
    {
        const std::string FILENAME = (Evolueur::TEMP_DIR / std::to_string(threadId)).string();
        Redis redis;
        std::optional<EvoluablePtr> ev;

        while(true)
        {
            /* récupération d'une entité */
            redis >> ev;
            if(!ev)
                continue;

            /* évolution de l'entité */
            do 
            {
                /* backup local de la forme intermédiaire */
                std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
                os << *ev;
            } while((*ev)->evoluer());

            // utilisation de l'entité
            (*ev)->utiliser();

            // suppression du fichier temp
            std::experimental::filesystem::remove(FILENAME);
        }
    }

  public:

    Evolueur()
    {
        std::experimental::filesystem::create_directory(Evolueur::TEMP_DIR);
    }

    void operator()()
    {
        std::vector<std::thread> pool;
        for(int threadId = 1; threadId <= Evolueur::POOL_SIZE; ++threadId)
            pool.push_back(std::thread(&Evolueur::callbackFn, this, threadId));

        for(auto& t : pool)
            t.join();
    }

    static void sigintHandler(int signum)
    {
        Redis redis;

        /* Copie des évoluables non complétés dans les inputs */
        EvoluablePtr ev;
        for(int threadId = 1 ; threadId <= Evolueur::POOL_SIZE ; ++threadId)
        {
            const std::experimental::filesystem::path FILE_PATH = 
                    Evolueur::TEMP_DIR / std::to_string(threadId);

            if(!std::experimental::filesystem::exists(FILE_PATH))
                continue;

            std::ifstream is(FILE_PATH.string(), std::ifstream::binary);
            is >> ev;
            redis << ev; //base 0 sélectionnée par défaut
        }

        std::experimental::filesystem::remove_all(Evolueur::TEMP_DIR);

        exit(signum);
    }
};

#endif