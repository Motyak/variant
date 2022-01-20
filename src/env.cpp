#include <env.h>

std::string env::GET_REDIS_HOST()
{
    char* host = std::getenv("REDIS_HOST");
    if(!host)
        return "127.0.0.1";
    return std::string(host);
}

std::string env::GET_REDIS_PORT()
{
    char* port = std::getenv("REDIS_PORT");
    if(!port)
        return "6379";
    return std::string(port);
}
