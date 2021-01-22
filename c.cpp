#include <iostream>
#include <variant>
#include <vector>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

struct Playlist
{
    struct InputParams
    {
        enum Quality { BASIC, GOOD };
        unsigned playlistId;
        Quality quality;
        Playlist* playlist;
    };

    struct HttpReq
    {
        enum Type { GET, POST };
        Type type;
        std::string url;
        std::string data;
        Playlist* playlist;
    };

    struct Json
    {
        std::string content;
        Playlist* playlist;
    };

    std::variant<InputParams,HttpReq,Json> form;

    Playlist(std::variant<InputParams,HttpReq,Json> initialForm) : form(initialForm){}
};

using PlaylistForm = std::variant<Playlist::InputParams,Playlist::HttpReq,Playlist::Json>;

/* UTILITY */
void print(const PlaylistForm& form)
{
    std::visit(
        overload {
            [](const Playlist::InputParams& params) { std::cout<<params.playlistId<<";"<<params.quality<<std::endl<<std::endl; },
            [](const Playlist::HttpReq& req) { std::cout<<(int)req.type<<";"<<req.url<<";"<<req.data<<std::endl<<std::endl; },
            [](const Playlist::Json& json) { std::cout<<json.content<<std::endl; }
        },
        form
    );
}

void evolve(PlaylistForm& form)
{
    std::visit(
        overload {
            [](Playlist::InputParams& params) {
                std::cout<<"InputParams:"<<std::endl;
                print(params.playlist->form);
                params.playlist->form = {Playlist::HttpReq{Playlist::HttpReq::Type::GET, "url", "data", params.playlist}};
            },
            [](Playlist::HttpReq& req) {
                std::cout<<"HttpReq:"<<std::endl;
                print(req.playlist->form);
                req.playlist->form = {Playlist::Json{"content", req.playlist}};
            },
            [](Playlist::Json& json) {
                std::cout<<"Json:"<<std::endl;
                print(json.playlist->form);
            }
        },
        form
    );
}

int main()
{
    // Playlist creation with its initial form
    Playlist playlist({Playlist::InputParams{1000, Playlist::InputParams::Quality::GOOD, &playlist}});

    // print playlist form and process from InputParams to HttpReq
    evolve(playlist.form);

    // print playlist form and process from HttpReq to Json
    evolve(playlist.form);

    // print playlist form
    evolve(playlist.form);
}