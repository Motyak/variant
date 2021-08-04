#ifndef _REDIS_PLUS_PLUS_BNY
#define _REDIS_PLUS_PLUS_BNY

#include <sw/redis++/redis++.h>

class _RedisPlusPlusBny
{
    std::unique_ptr<sw::redis::Redis> redis;

  public:
  
    _RedisPlusPlusBny(const std::string& url);
    void command(std::string&&, int);
    void set(const std::string& key, const std::string& val);
    std::optional<std::string> eval(const std::string&);
};

#endif
