#ifndef REDIS_H
#define REDIS_H

#include <env.h>
#include <Evoluable.hpp>

#include <sw/redis++/redis++.h>

#include <mutex>

class Redis
{
    inline static std::mutex mut;

    const std::string ATOMIC_POP = "local key = redis.call('RANDOMKEY'); if key then redis.call('DEL', key) end; return key";

    std::unique_ptr<sw::redis::Redis> connexion;

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
        Output(EvoluablePtr ev);
    };

    Redis();
    Redis(std::string host, std::string port);

    void changerBase(Base b);
    void ajouter(const EvoluablePtr& ev);
    std::optional<EvoluablePtr> recuperer();
};

Redis& operator<<(Redis& redis, const EvoluablePtr& ev);
Redis& operator<<(Redis& redis, const std::vector<EvoluablePtr>& evs);
Redis& operator<<(Redis& redis, const Redis::Output& out);
Redis& operator>>(Redis& redis, std::optional<EvoluablePtr>& ev);

#endif
