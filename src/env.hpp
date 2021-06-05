#ifndef ENV_HPP
#define ENV_HPP

#include <string>

namespace env
{
    std::string GET_REDIS_HOST()
    {
        char* host = std::getenv("REDIS_HOST");
        if(!host)
            return "127.0.0.1";
        return std::string(host);
    }

    std::string GET_REDIS_PORT()
    {
        char* port = std::getenv("REDIS_PORT");
        if(!port)
            return "3679";
        return std::string(port);
    }
}

#endif