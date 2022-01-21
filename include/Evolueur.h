#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include <thread>
#include <experimental/filesystem>

class Evolueur
{
    inline static const int POOL_SIZE = std::thread::hardware_concurrency(); 
    inline static const std::experimental::filesystem::path TEMP_DIR = std::experimental::filesystem::temp_directory_path() / "evolueur";

    void callbackFn(int threadId);

  public:

    Evolueur();

    void operator()();

    static void sigintHandler(int signum);
};

#endif
