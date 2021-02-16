#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include <iostream>
#include <variant>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

template<typename... Types>
struct Evoluable
{
    std::variant<Types...> forme;

    Evoluable(std::variant<Types...> formeInitiale) : forme(formeInitiale){}

    virtual void evolve() = 0;

    friend std::ostream& operator<<(std::ostream& out, const Evoluable& e)
    {
        e.insert(out);
        return out;
    }
    friend std::istream& operator>>(std::istream& in, Evoluable& e)
    {
        e.extract(in);
        return in;
    }

  private:
    virtual void insert(std::ostream& out) const
    {
        std::size_t s = sizeof(this->forme);
        // std::cout<<"size of next elem : "<<s<<std::endl;
        out.write((char*)&s, sizeof(std::size_t));
        out.write((char*)&this->forme, s);
    }
    virtual void extract(std::istream& in)
    {
        std::size_t s;
        in.read((char*)&s, sizeof(std::size_t));
        // std::cout<<"size of next elem : "<<s<<std::endl;
        in.read((char*)&this->forme, s);
    }
};

#endif