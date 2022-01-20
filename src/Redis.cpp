#include <Redis.h>

Redis::Output::Output(EvoluablePtr ev) : ev(ev){}

Redis::Redis() : Redis(env::GET_REDIS_HOST(), env::GET_REDIS_PORT()){}

Redis::Redis(std::string host, std::string port)
{
    this->connexion = std::make_unique<sw::redis::Redis>("tcp://" + host + ":" + port);
}

void Redis::changerBase(Base b)
{
    this->connexion->command("SELECT", (int)b);
}

void Redis::ajouter(const EvoluablePtr& ev)
{
    std::ostringstream oss;
    oss << ev;
    this->connexion->set(oss.str(), "");
}

std::optional<EvoluablePtr> Redis::recuperer()
{
    Redis::mut.lock();
    auto key = this->connexion->eval<sw::redis::OptionalString>(this->ATOMIC_POP, {}, {});
    Redis::mut.unlock();
    if(!key)
        return {};
    EvoluablePtr ev;
    std::istringstream(*key) >> ev;
    return ev;
}

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
