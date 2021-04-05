#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include "Evoluable.hpp"

#include <sw/redis++/redis++.h>

#include <sstream>
#include <thread>
#include <mutex>
#include <experimental/filesystem>
#include <fstream>

void operator<<(sw::redis::Redis& redis, const EvoluablePtr& ev)
{
    std::ostringstream oss;
    oss << ev;
    redis.set(oss.str(), "");
}

class Evolueur
{
    std::experimental::filesystem::path TEMP_DIR;
    std::mutex mut;

    void callbackFn(int threadId)
    {
        const std::string FILENAME = (this->TEMP_DIR / std::to_string(threadId)).string();
        auto redis = sw::redis::Redis("tcp://" + Evolueur::HOST + ":" + Evolueur::PORT);
        EvoluablePtr ev;

        while(true)
        {
            /* récupération d'une entité */
            redis.command("SELECT", "0");
            this->mut.lock();
            auto key = sw::redis::reply::parse<sw::redis::OptionalString>(*redis.command("RANDOMKEY"));
            if(!key)
            {
                this->mut.unlock();
                continue;
            }
            redis.command("DEL", *key);
            this->mut.unlock();
            std::istringstream iss(*key);
            iss >> ev;

            /* évolution de l'entité */
            do 
            {
                /* backup local de la forme intermédiaire */
                std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
                os << ev;
            } while(ev->evoluer());

            /* sauvegarde de la forme finale */
            redis.command("SELECT", "1");
            redis << ev;
        }
    }

  public:
    // inline static const std::string HOST = "127.0.0.1";
    inline static const std::string HOST = "192.168.56.109";
    inline static const std::string PORT = "6379";

    Evolueur()
    {
        this->TEMP_DIR = std::experimental::filesystem::temp_directory_path()
                / "evolueur";
        std::experimental::filesystem::create_directory(this->TEMP_DIR);
    }

    void operator()()
    {
        const int POOL_SIZE = std::thread::hardware_concurrency();
        std::vector<std::thread> pool;
        for(int threadId = 1; threadId <= POOL_SIZE; ++threadId)
            pool.push_back(std::thread(&Evolueur::callbackFn, this, threadId));

        for(auto& t : pool)
            t.join();
    }

    static void sigintHandler(int signum)
    {
        const std::experimental::filesystem::path TEMP_DIR = 
                std::experimental::filesystem::temp_directory_path() 
                / "evolueur";
        auto redis = sw::redis::Redis("tcp://" + Evolueur::HOST + ":" + Evolueur::PORT);

        /* Copie des évoluables non complétés dans les inputs */
        EvoluablePtr ev;
        const int POOL_SIZE = std::thread::hardware_concurrency();
        for(int threadId = 1 ; threadId <= POOL_SIZE ; ++threadId)
        {
            const std::experimental::filesystem::path FILE_PATH = 
                    TEMP_DIR / std::to_string(threadId);

            if(!std::experimental::filesystem::exists(FILE_PATH))
                continue;
            else
            {
                std::ifstream is(FILE_PATH.string(), std::ifstream::binary);
                is >> ev;
                redis << ev; //base 0 sélectionnée par défaut
            }
        }

        std::experimental::filesystem::remove_all(TEMP_DIR);

        exit(signum);
    }
};

#endif