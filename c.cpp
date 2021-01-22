#include <iostream>
#include <variant>
#include <vector>

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

class PlaylistProcessor
{
  public:
    virtual void operator()(Playlist::InputParams& params) = 0;
    virtual void operator()(Playlist::HttpReq& params) = 0;
    virtual void operator()(Playlist::Json& params) = 0;
};

class Processes : public PlaylistProcessor
{
  public:
    void operator()(Playlist::InputParams& params) override
    {
        std::cout<<"InputParams:"<<std::endl;
        afficher(std::get<Playlist::InputParams>(params.playlist->form));
        params.playlist->form = {Playlist::HttpReq{params.playlist, Playlist::HttpReq::Type::GET, "url", "data"}};
    }

    void operator()(Playlist::HttpReq& req) override
    {
        std::cout<<"HttpReq:"<<std::endl;
        afficher(std::get<Playlist::HttpReq>(req.playlist->form));
        req.playlist->form = {Playlist::Json{req.playlist, "content"}};
    }

    void operator()(Playlist::Json& json) override
    {
        std::cout<<"Json:"<<std::endl;
        afficher(std::get<Playlist::Json>(json.playlist->form));
    }
};

int main()
{
    // Playlist creation with its initial form
    Playlist playlist = {{&playlist, 1000, Playlist::InputParams::Quality::GOOD}};

    // print playlist form and process from InputParams to HttpReq
    std::visit(Processes{}, playlist.form);

    // print playlist form and process from HttpReq to Json
    std::visit(Processes{}, playlist.form);

    // print playlist form
    std::visit(Processes{}, playlist.form);
}