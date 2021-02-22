#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include "Evoluable.hpp"

#include <sw/redis++/redis++.h>

#include <sstream>
#include <thread>
#include <mutex>

void operator<<(sw::redis::Redis& redis, const EvoluablePtr& ev)
{
    std::ostringstream oss;
    oss << ev;
    redis.set(oss.str(), "");
}

class Evolueur
{
    std::mutex mut;

    void callbackFn(const int& dbId)
    {
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
                return;
            }
            redis.command("DEL", *key);
            this->mut.unlock();
            std::istringstream iss(*key);
            iss >> ev;

            /* évolution de l'entité */
            redis.command("SELECT", std::to_string(dbId));
            do 
            {
                /* sauvegarde de la forme actuelle */
                redis.command("FLUSHDB");
                redis << ev;
            } while(ev->evoluer());

            /* sauvegarde de la forme finale */
            redis.command("SELECT", "1");
            redis << ev;
        }
    }

  public:
    inline static const std::string HOST = "127.0.0.1";
    inline static const std::string PORT = "6379";

    void operator()()
    {
        const int POOL_SIZE = std::thread::hardware_concurrency();
        std::vector<std::thread> pool;
        for(int i = 0; i < POOL_SIZE; ++i)
            pool.push_back(std::thread(&Evolueur::callbackFn, this, 2 + i));

        for(auto& t : pool)
            t.join();
    }

    static void sigintHandler(int signum)
    {
        std::cout<<"sigint"<<std::endl;

        /* Copie des évoluables non complétés dans les inputs */
        auto redis = sw::redis::Redis("tcp://" + Evolueur::HOST + ":" + Evolueur::PORT);
        for(int i = 2 ; i <= 5 ; ++i)
        {
            redis.command("SELECT", std::to_string(i));
            auto key = sw::redis::reply::parse<sw::redis::OptionalString>(*redis.command("RANDOMKEY"));
            if(!key)
                continue;
            redis.command("MOVE", *key, "0");
        }

        exit(signum);
    }
};

#endif