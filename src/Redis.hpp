#ifndef REDIS_HPP
#define REDIS_HPP

#include "env.hpp"
#include "Evoluable.hpp"
#include "_RedisPlusPlusBny.h"

#include <mutex>

class Redis
{
    inline static std::mutex mut;

    const std::string ATOMIC_POP = "local key = redis.call('RANDOMKEY'); if key then redis.call('DEL', key) end; return key";

    std::unique_ptr<_RedisPlusPlusBny> connexion;

  public:

    enum Base
    {
        INPUTS, //par défaut
        OUTPUTS
    };

    // on peut insérer Output dans Redis pour push dans la base outputs
    struct Output
    {
        EvoluablePtr ev;
        Output(EvoluablePtr ev) : ev(ev){}
    };

    Redis(std::string host, std::string port)
    {
        this->connexion = std::make_unique<_RedisPlusPlusBny>("tcp://" + host + ":" + port);
    }
    
    Redis() : Redis(env::GET_REDIS_HOST(), env::GET_REDIS_PORT()){}

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
        auto key = this->connexion->eval(this->ATOMIC_POP);
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

Redis& operator<<(Redis& redis, const std::vector<EvoluablePtr>& evs)
{
    for(const auto& ev : evs)
        redis << ev;
    return redis;
}

Redis& operator<<(Redis& redis, const Redis::Output& out)
{
    redis.changerBase(Redis::OUTPUTS);
    redis << out.ev;
    redis.changerBase(Redis::INPUTS); // on remet par défaut
    return redis;
}

Redis& operator>>(Redis& redis, std::optional<EvoluablePtr>& ev)
{
    ev = redis.recuperer();
    return redis;
}

#endif
