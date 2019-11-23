#include "include/Watchable.h"
#include  <sstream>

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
    const std::vector<std::string> Watchable::get_tags() const {return this->tags;}
    const long Watchable::get_id() const {return this->id;}
    const int Watchable::get_length() const {return this->length;}
    const std::string Watchable::concat_tags() const {
        std::string concat_tags="[";
        for(int i=0;i<this->tags.size();i++){
            concat_tags.append(tags.at(i));
            concat_tags.append(",");
        }
        concat_tags.at(concat_tags.size()-1)=']';
        return concat_tags;
    }
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
    const std::string Movie::get_name() {return this->name;}
    std::string Movie::toString() const {
        std::ostringstream strout;
        strout<< std::to_string(this->get_id())<<" " << this->name << " " << this->get_length()<< "minutes";
        return strout.str().append(this->concat_tags());
    }
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

    std::string Episode::toString() const {
        std::ostringstream strout;
        strout<< std::to_string(this->get_id())<<". " << this->get_seriesName() << " S"<<std::to_string(this->get_season()) <<"E"<<std::to_string(this->get_episode())<<" " <<this->get_length()<<" minutes";
        return strout.str().append(this->concat_tags());
    }
    Watchable* Episode::getNextWatchable(Session &) const {}//TODO:implement
    const std::string Episode::get_seriesName() const {return this->seriesName;}
    int Episode::get_episode() const {return this->episode;}
    int Episode::get_season() const {return this->season;}
    long Episode::get_next_EpisodeId() { return this->nextEpisodeId;};


//endregion

