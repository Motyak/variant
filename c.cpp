#include <iostream>
#include <variant>
#include <vector>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

struct Playlist
{
    struct Form { Playlist* playlist; };

    struct InputParams : public Form
    {
        enum Quality { BASIC, GOOD };
        unsigned playlistId;
        Quality quality;
    };

    struct HttpReq : public Form
    {
        enum Type { GET, POST };
        Type type;
        std::string url;
        std::string data;
    };

    struct Json : public Form { std::string content; };

    std::variant<InputParams,HttpReq,Json> form;

    Playlist(InputParams initialForm) : form(initialForm){}
};

/* UTILITIES */
void afficher(const Playlist::InputParams& params)
{
    std::cout<<params.playlistId<<";"<<params.quality<<std::endl<<std::endl;
}
void afficher(const Playlist::HttpReq& req)
{
    std::cout<<(int)req.type<<";"<<req.url<<";"<<req.data<<std::endl<<std::endl;
}
void afficher(const Playlist::Json& json)
{
    std::cout<<json.content<<std::endl;
}

const auto processor = overload {
    [](Playlist::InputParams& params){ params.playlist->form = {Playlist::HttpReq{params.playlist, Playlist::HttpReq::Type::GET, "url", "data"}}; },
    [](Playlist::HttpReq& req){ req.playlist->form = {Playlist::Json{req.playlist, "content"}}; },
    [](Playlist::Json& json){}
};

// Playlist creation with its initial form
Playlist playlist = {{&playlist, 1000, Playlist::InputParams::Quality::GOOD}};

// supposed to be a method, OOP
void evolve()
{
    std::visit(processor, playlist.form);
}

int main()
{
    try{ afficher(std::get<Playlist::Json>(playlist.form)); }
    catch(std::exception e){ std::cout<<"y'a rien"<<std::endl; }

    evolve();

    try{ afficher(std::get<Playlist::Json>(playlist.form)); }
    catch(std::exception e){ std::cout<<"y'a rien"<<std::endl; }

    evolve();

    try{ afficher(std::get<Playlist::Json>(playlist.form)); }
    catch(std::exception e){ std::cout<<"y'a rien"<<std::endl; }

    evolve();

    try{ afficher(std::get<Playlist::Json>(playlist.form)); }
    catch(std::exception e){ std::cout<<"y'a rien"<<std::endl; }

    // // Playlist creation with its initial form
    // Playlist playlist = {{&playlist, 1000, Playlist::InputParams::Quality::GOOD}};

    // // print playlist form and process from InputParams to HttpReq
    // std::visit(Processes{}, playlist.form);

    // // print playlist form and process from HttpReq to Json
    // std::visit(Processes{}, playlist.form);

    // // print playlist form
    // std::visit(Processes{}, playlist.form);
}