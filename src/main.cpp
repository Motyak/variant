#include <Redis.h>
#include <Evolueur.h>
#include <Entite.hpp>

#include <csignal>

/* overload pattern for in-place visitation using std::visit */
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

Redis& operator<<(Redis& redis, std::variant<Entite,Entite2> e)
{
    return std::visit(overload {
        [&](const Entite& e1) -> Redis& {
            return redis << std::make_shared<Entite>(e1);
        },
        [&](const Entite2& e2) -> Redis& {
            return redis << std::make_shared<Entite2>(e2);
        }
    }, e);
}

// g++ -o evolueur src/main.cpp lib/libevo.so --std=c++17 -I include -lstdc++fs -pthread
int main()
{
    /* ECRITURE */
    {
        Redis redis;
        redis << Entite{91} << Entite{13.37f} << Entite{"unechaine,unechaine,unechaine"}
              << Entite{10} << Entite{10.37f} << Entite{"unechaine10,unechaine10,unechaine10"}
              << Entite2{84.0} << Entite2{'a'} << Entite2{79.0}
              << Entite2{'b'} << Entite2{77.0} << Entite2{'c'}
              << Entite{910} << Entite{130.37f} << Entite{"_unechaine,unechaine,unechaine"}
              << Entite{100} << Entite{100.37f} << Entite{"_unechaine10,unechaine10,unechaine10"}
              << Entite2{840.0} << Entite2{'A'} << Entite2{790.0}
              << Entite2{'B'} << Entite2{770.0} << Entite2{'C'};
    }

    /* EVOLUTION */
    {
        Evolueur e;
        signal(SIGINT, Evolueur::sigintHandler);
        e();
    }
}
