#ifndef EVOLUABLE_H
#define EVOLUABLE_H

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/variant.hpp>

template<class... Ts> struct Evolution : Ts... { using Ts::operator()...; };
template<class... Ts> Evolution(Ts...) -> Evolution<Ts...>;

struct Evoluable;
using EvoluablePtr = std::shared_ptr<Evoluable>;
using OptionalEvoluablePtrs = std::optional<std::vector<EvoluablePtr>>;
struct Evoluable
{
    virtual bool evoluer() = 0;
    virtual OptionalEvoluablePtrs split();

    template<typename... Types>
    struct Forme
    {
        std::variant<Types...> forme;

        Forme() = default;
        Forme(const std::variant<Types...>& formeInitiale);

        template<class Archive>
        void serialize(Archive& ar);
    };
};
CEREAL_REGISTER_TYPE(Evoluable);

std::ostream& operator<<(std::ostream& os, const EvoluablePtr& ev);
std::istream& operator>>(std::istream& is, EvoluablePtr& ev);

#endif
