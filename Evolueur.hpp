#ifndef EVOLUEUR_H
#define EVOLUEUR_H

#include <iostream>

template<typename T>
struct Evoluable
{
    T forme;

    Evoluable(T formeInitiale) : forme(formeInitiale){}

    virtual void evolve() = 0;

    friend std::ostream& operator<<(std::ostream& out, const Evoluable& e) { e.insert(out); }
    friend std::istream& operator>>(std::istream& in, Evoluable& e) { e.extract(in); }

  private:
    virtual void insert(std::ostream& out) const { out.write((char*)&this->forme, sizeof(this->forme)); }
    virtual void extract(std::istream& in) { in.read((char*)&this->forme, sizeof(this->forme)); }
};

class Evolueur;

#endif