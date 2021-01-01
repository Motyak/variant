#include <iostream>
#include <variant>
#include <vector>

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

struct Processes
{
    void operator()(Playlist::InputParams& params)
    {
        std::cout<<"InputParams:"<<std::endl;
        afficher(std::get<Playlist::InputParams>(params.playlist->form));
        params.playlist->form = {Playlist::HttpReq{Playlist::HttpReq::Type::GET, "url", "data", params.playlist}};
    }

    void operator()(Playlist::HttpReq& req)
    {
        std::cout<<"HttpReq:"<<std::endl;
        afficher(std::get<Playlist::HttpReq>(req.playlist->form));
        req.playlist->form = {Playlist::Json{"content", req.playlist}};
    }

    void operator()(Playlist::Json& json)
    {
        std::cout<<"Json:"<<std::endl;
        afficher(std::get<Playlist::Json>(json.playlist->form));
    }
};

int main()
{
    // Playlist creation with its initial form
    Playlist playlist({Playlist::InputParams{1000, Playlist::InputParams::Quality::GOOD, &playlist}});

    // print playlist form and process from InputParams to HttpReq
    std::visit(Processes{}, playlist.form);

    // print playlist form and process from HttpReq to Json
    std::visit(Processes{}, playlist.form);

    // print playlist form
    std::visit(Processes{}, playlist.form);
}