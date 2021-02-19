#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/variant.hpp"

#include <fstream>
#include <thread>
#include <mutex>
#include <functional>

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

class Evolueur
{
    void callbackFn(std::istream& is, std::ostream& os, std::mutex& read, std::mutex& write)
    {
        EvoluablePtr ev;
        while(true)
        {
            /* extraction du prochain élément */
            read.lock();
            if(is.peek() == std::ifstream::traits_type::eof())
            {
                read.unlock();
                return; // ou 'continue' si on veut que ça continue de tourner
            }
            is >> ev;
            read.unlock();

            /* operation sur l'element */
            do 
            {
                /* sauvegarde de la forme actuelle */
                write.lock();
                os << ev;
                write.unlock();
            } while(ev->evoluer());

            /* sauvegarde de la forme finale */
            write.lock();
            os << ev;
            write.unlock();
        }
    }

  public:
    void operator()(const std::string& inputFilename, const std::string& outputFilename)
    {
        const int POOL_SIZE = std::thread::hardware_concurrency();
        std::ifstream is(inputFilename, std::ifstream::binary);
        std::ofstream os(outputFilename, std::ofstream::binary | std::ofstream::trunc);
        std::mutex read, write;
        std::vector<std::thread> pool;
        for(int i = 0; i < POOL_SIZE; ++i)
            pool.push_back(std::thread(&Evolueur::callbackFn, this, std::ref(is), std::ref(os), std::ref(read), std::ref(write)));

        for(auto& t : pool)
            t.join();
    }
};

#endif