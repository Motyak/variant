#include <thread>
#include <mutex>
// #include <vector>

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

class ThreadPool
{
    const std::thread[] workers;
    std::queue<Playlist>

  public:
    void evolve()
};


int main()
{

}