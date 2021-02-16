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

//     virtual void evolve() = 0;

    friend std::ostream& operator<<(std::ostream& out, const Evoluable& e)
    {
        e.insert(out);
        return out;

        // return std::visit(overload {
            // [&](const std::string& str) -> std::ostream& {
            //     std::size_t s = sizeof(char) * str.length();
            //     out.write((char*)&s, sizeof(std::size_t));
            //     return out << str;
            // },
        //     [&](auto other) -> std::ostream& {
        //         e.insert(out);
        //         return out;
        //     }
        // }, e.forme);
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
        std::cout<<"size of next elem : "<<s<<std::endl;
        out.write((char*)&s, sizeof(std::size_t));

        // affichage debug
        std::visit(overload {
            [](const int& d) { std::cout<<d<<std::endl; },
            [](const float& f) { std::cout<<f<<std::endl; },
            [](const std::string& str) { std::cout<<str<<std::endl; },
            [](auto onsaitpas){ std::cout<<"onsaitpas"<<std::endl; }
        }, this->forme);

        out.write((char*)&this->forme, s);
    }
    virtual void extract(std::istream& in)
    {
        std::size_t s;
        in.read((char*)&s, sizeof(std::size_t));
        std::cout<<"size of next elem : "<<s<<std::endl;
        in.read((char*)&this->forme, s);
    }
};

#endif