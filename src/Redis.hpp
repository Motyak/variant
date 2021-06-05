#ifndef REDIS_HPP
#define REDIS_HPP

#include "env.hpp"
#include "Evoluable.hpp"

#include <sw/redis++/redis++.h>

#include <mutex>

class Redis
{
    inline static std::mutex mut;

    const std::string SAFE_GET = "local key = redis.call('RANDOMKEY'); if key then redis.call('DEL', key) end; return key";

    std::unique_ptr<sw::redis::Redis> connexion;

  public:

    enum Base
    {
        INPUTS,
        OUTPUTS
    };

    Redis()
    {
        this->connexion = std::make_unique<sw::redis::Redis>("tcp://" + env::GET_REDIS_HOST() + ":" + env::GET_REDIS_PORT());
    }

    void changerBase(Base b)
    {
        this->connexion->command("SELECT", (int)b);
    }

    void ajouter(const EvoluablePtr& ev)
    {
        std::ostringstream oss;
        oss << ev;
        this->connexion->set(oss.str(), "");
    }

    std::optional<EvoluablePtr> recuperer()
    {
        Redis::mut.lock();
        auto key = this->connexion->eval<sw::redis::OptionalString>(this->SAFE_GET, {}, {});
        Redis::mut.unlock();
        if(!key)
            return {};
        EvoluablePtr ev;
        std::istringstream(*key) >> ev;
        return ev;
    }
};

Redis& operator<<(Redis& redis, const EvoluablePtr& ev)
{
    redis.ajouter(ev);
    return redis;
}

Redis& operator>>(Redis& redis, std::optional<EvoluablePtr>& ev)
{
    ev = redis.recuperer();
    return redis;
}

#endif