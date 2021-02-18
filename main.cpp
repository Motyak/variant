#include "Entite.hpp"

// g++ main.cpp -o evolueur -std=c++17 -pthread
int main(int argc, char* argv[])
{
    const std::string INPUT_FILENAME = argv[1];
    const std::string OUTPUT_FILENAME = argv[2];

    /* ECRITURE */
    {
        std::ofstream os(INPUT_FILENAME, std::ofstream::binary | std::ofstream::trunc);
        for(const auto& e : std::vector<EvoluablePtr>{
            std::make_shared<Entite>(91), std::make_shared<Entite>(13.37f),
            std::make_shared<Entite>("unechaine,unechaine,unechaine"),
            std::make_shared<Entite>(10), std::make_shared<Entite>(10.37f),
            std::make_shared<Entite>("unechaine10,unechaine10,unechaine10"),
            std::make_shared<Entite2>(84.0), std::make_shared<Entite2>('a'),
            std::make_shared<Entite2>(79.0), std::make_shared<Entite2>('b'),
            std::make_shared<Entite2>(77.0), std::make_shared<Entite2>('c'),
        })
            os << e;
    }

    /* EVOLUTION */
    {
        Evolueur e;
        e(INPUT_FILENAME, OUTPUT_FILENAME);
    }
}