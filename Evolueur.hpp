#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include <sw/redis++/redis++.h>
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/variant.hpp"

#include <sstream>
#include <thread>
#include <mutex>

template<class... Ts> struct Evolution : Ts... { using Ts::operator()...; };
template<class... Ts> Evolution(Ts...) -> Evolution<Ts...>;

struct Evoluable
{
    virtual bool evoluer() = 0;

    template<typename... Types>
    struct Forme
    {
        std::variant<Types...> forme;

        Forme() = default;
        Forme(const std::variant<Types...>& formeInitiale) : forme(formeInitiale){}

        template<class Archive>
        void serialize(Archive& ar) { ar(forme); }
    };
};
CEREAL_REGISTER_TYPE(Evoluable);

using EvoluablePtr = std::shared_ptr<Evoluable>;

std::ostream& operator<<(std::ostream& os, const EvoluablePtr& ev)
{
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(ev);
    return os;
}

std::istream& operator>>(std::istream& is, EvoluablePtr& ev)
{
    cereal::BinaryInputArchive iarchive(is);
    iarchive(ev);
    return is;
}

void operator<<(sw::redis::Redis& redis, const EvoluablePtr& ev)
{
    std::ostringstream oss;
    oss << ev;
    redis.set(oss.str(), "");
}

class Evolueur
{
    const std::string host, port;
    std::mutex mut;

    void callbackFn(const int& i)
    {
        auto redis = sw::redis::Redis("tcp://" + this->host + ":" + this->port);
        EvoluablePtr ev;

        while(true)
        {
            std::unordered_set<std::string> keys;

            /* extraction du prochain élément */
            redis.command("SELECT", "0");
            this->mut.lock();
            redis.scan(0, 1, std::inserter(keys, keys.begin()));
            if(keys.empty())
            {
                this->mut.unlock();
                return;
            }
            std::istringstream iss(*redis.get(*keys.begin()));
            redis.command("DEL", *keys.begin());
            this->mut.unlock();
            iss >> ev;

            /* operation sur l'element */
            redis.command("SELECT", std::to_string(i));
            do 
            {
                /* sauvegarde de la forme actuelle */
                redis << ev;
            } while(ev->evoluer());
            redis.command("DEL", *keys.begin());

            /* sauvegarde de la forme finale */
            redis.command("SELECT", "1");
            redis << ev;
        }
    }

  public:
    Evolueur(const std::string& host = "127.0.0.1", const std::string& port = "6379") : host(host), port(port){}
    
    void operator()()
    {
        const int POOL_SIZE = std::thread::hardware_concurrency();
        std::mutex mut;
        std::vector<std::thread> pool;
        for(int i = 0; i < POOL_SIZE; ++i)
            pool.push_back(std::thread(&Evolueur::callbackFn, this, 10 + i));

        for(auto& t : pool)
            t.join();
    }
};

#endif