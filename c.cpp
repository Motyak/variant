#include <iostream>
#include <variant>
#include <vector>

struct Numbers
{
    std::vector<unsigned> n;
};

using Text = std::variant<std::string,Numbers>;

struct Processes
{
    Numbers operator()(const std::string& text)
    {
        std::cout<<"string to numbers"<<std::endl;
        std::vector<unsigned> res;
        for(const auto& c : text)
            res.push_back((int)c);
        return Numbers{res};
    }

    Numbers operator()(const Numbers& text)
    {
        std::cout<<"do nothing"<<std::endl;
        return Numbers{};
    }
};


int main()
{
    Text text{"test"};
    text = std::visit(Processes{}, text);
    for(const auto& e : std::get<Numbers>(text).n)
        std::cout<<e<<std::endl;
}