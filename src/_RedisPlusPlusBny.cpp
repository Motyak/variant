#include "_RedisPlusPlusBny.h"

_RedisPlusPlusBny::_RedisPlusPlusBny(const std::string& url)
{
    this->redis = std::make_unique<sw::redis::Redis>(url);
}

void _RedisPlusPlusBny::command(std::string&& str, int i)
{
    this->redis->command(str, i);
}

void _RedisPlusPlusBny::set(const std::string& key, const std::string& val)
{
    this->redis->set(key, val);
}

std::optional<std::string> _RedisPlusPlusBny::eval(const std::string& str)
{
    return this->redis->eval<sw::redis::OptionalString>(str, {}, {});
}
