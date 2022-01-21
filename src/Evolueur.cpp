#include <Evolueur.h>

#include <Evoluable.hpp>
#include <Redis.h>

#include <fstream>

void Evolueur::callbackFn(int threadId)
{
    const std::string FILENAME = (Evolueur::TEMP_DIR / std::to_string(threadId)).string();
    Redis redis;
    std::optional<EvoluablePtr> ev;

    while(true)
    {
        /* récupération d'une entité */
        redis >> ev;
        if(!ev)
            continue;

        /* évolution de l'entité */
        do
        {
            /* backup local de la forme intermédiaire */
            std::ofstream os(FILENAME, std::ofstream::binary | std::ofstream::trunc);
            os << *ev;
        } while((*ev)->evoluer());

        /* push split ou forme finale */
        auto split = (*ev)->split();
        if(split)
            redis << *split;
        else
            redis << Redis::Output{*ev};

        // suppression du fichier temp
        std::experimental::filesystem::remove(FILENAME);
    }
}

Evolueur::Evolueur()
{
    std::experimental::filesystem::create_directory(Evolueur::TEMP_DIR);
}

void Evolueur::operator()()
{
    std::vector<std::thread> pool;
    for(int threadId = 1; threadId <= Evolueur::POOL_SIZE; ++threadId)
        pool.push_back(std::thread(&Evolueur::callbackFn, this, threadId));

    for(auto& t : pool)
        t.join();
}

void Evolueur::sigintHandler(int signum)
{
    Redis redis;

    /* Copie des évoluables non complétés dans les inputs */
    EvoluablePtr ev;
    for(int threadId = 1 ; threadId <= Evolueur::POOL_SIZE ; ++threadId)
    {
        const std::experimental::filesystem::path FILE_PATH = 
                Evolueur::TEMP_DIR / std::to_string(threadId);

        if(!std::experimental::filesystem::exists(FILE_PATH))
            continue;

        std::ifstream is(FILE_PATH.string(), std::ifstream::binary);
        is >> ev;
        redis << ev; //base 0 sélectionnée par défaut
    }

    std::experimental::filesystem::remove_all(Evolueur::TEMP_DIR);

    exit(signum);
}
