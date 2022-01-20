#include <Evoluable.h>

OptionalEvoluablePtrs Evoluable::split() { return {}; }

template<typename... Types>
Evoluable::Forme<Types...>::Forme(const std::variant<Types...>& formeInitiale) : forme(formeInitiale){}

template<typename... Types>
template<class Archive>
void Evoluable::Forme<Types...>::serialize(Archive& ar) { ar(forme); }

std::ostream& operator<<(std::ostream& os, const EvoluablePtr& ev)
{
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(ev);
    return os;
}

std::istream& operator>>(std::istream& is, EvoluablePtr& ev)
{
    cereal::BinaryInputArchive iarchive(is);
    iarchive(ev);
    return is;
}
