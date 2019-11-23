#include "include/Watchable.h"

using namespace std;

//region Watchable - abstract

    //Constructors:
    Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length) , tags(tags) {}
    Watchable::Watchable(const Watchable& other): id(other.id), length(other.length){
        for(int i=0; i<other.tags.size();i++){

            tags.push_back(other.tags[i]);
        }
    }
    //Methods:
    const std::vector<std::string> Watchable::get_tags() {return this->tags;}
    const long Watchable::get_id() {return this->id;}
    const int Watchable::get_length() {return this->length;}
//endregion

//region Movie

    //Constructors
    Movie::Movie(long id, const std::string &name, int length, const std::vector<std::string> &tags): Watchable(id,length,tags), name(name) {}
    Movie::Movie(const Movie &other):Watchable(other), name(other.name) {}
    Movie::~Movie() {}

    //Methods:
    Movie* Movie::clone()
    {
        return new Movie(*this);
    }
    std::string Movie::get_name() {return this->name;}
    std::string Movie::toString() const {}
    Watchable* Movie::getNextWatchable(Session &) const {}

//endregion

//region Episode

    //Constructors:
    Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode,
                     const std::vector<std::string> &tags):Watchable(id,length,tags), seriesName(seriesName), season(season), episode(episode)
    {
        //TODO: get next episode id if available
    }
    Episode::Episode(const Episode &other) : Watchable(other), seriesName(other.seriesName), season(other.season), episode(other.episode),nextEpisodeId(other.nextEpisodeId){}

    //Destructor:
    Episode::~Episode()
    {
        //TODO: implement
    }

    //Methods:
    Episode* Episode::clone()
    {
        return new Episode(*this);
    }
    std::string Episode::toString() const {}//TODO:implement
    Watchable* Episode::getNextWatchable(Session &) const {}//TODO:implement
    const std::string Episode::get_seriesName() {return this->seriesName;}
    int Episode::get_episode() {return this->episode;}
    int Episode::get_season() {return this->season;}
    long Episode::get_next_EpisodeId() { return this->nextEpisodeId;};


//endregion

