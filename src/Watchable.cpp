#include "include/Watchable.h"
#include  <sstream>

using namespace std;

//region Watchable - abstract

    //Constructors:
    Watchable::Watchable(long id, int length, const vector<string> &tags) : id(id), length(length) , tags(tags) {} //parameter constructor
    Watchable::Watchable(const Watchable& other): id(other.get_id()), length(other.get_length()),tags(other.get_tags()){} //copy constructor
    Watchable::Watchable(Watchable &&other):id(other.get_id()), length(other.get_length()),tags(other.get_tags())//Move constructor
    {
        other.tags.clear();
        //TODO: might change the length in order to know that we move this watchable object

    }
    //Methods:
    vector<string> Watchable::get_tags() const {return this->tags;}
    long Watchable::get_id() const {return this->id;}
    int Watchable::get_length() const {return this->length;}
    string Watchable::concat_tags() const {
        string concat_tags="[";
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
    Movie::Movie(long id, const std::string &name, int length, const vector<string> &tags): Watchable(id,length,tags), name(name) {}
    Movie::Movie(const Movie &other):Watchable(other), name(other.name) {}
    Movie::Movie(Movie &&other):Watchable(other&) ,name(other.name)
    {
        other.get_tags().clear();
    }
//Methods:
    Movie* Movie::clone()
    {
        return new Movie(*this);
    }
    string Movie::get_name() {return this->name;}
    string Movie::toString() const {
        string toReturn="";
        toReturn.append(reinterpret_cast<const char *>(this->get_id()));
        toReturn.append(name);
        toReturn.append(reinterpret_cast<const char *>(this->get_length()));
        toReturn.append("minutes");
        toReturn.append(this->concat_tags());

        return toReturn;

    }
    Watchable* Movie::getNextWatchable(Session &) const {}

//endregion

//region Episode

    //Constructors:
    Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
                     const vector<string> &tags,long nextEpisodeId ):Watchable(id,length,tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(nextEpisodeId) { }
    Episode::Episode(const Episode &other) : Watchable(other), seriesName(other.seriesName), season(other.season), episode(other.episode),nextEpisodeId(other.nextEpisodeId){}
    Episode::Episode(Episode &&other):Watchable(other&),seriesName(other.get_seriesName()), season(other.get_season()),episode(other.get_episode()), nextEpisodeId(other.get_next_EpisodeId()){
    }
    //Methods:
    Episode* Episode::clone()
    {
        return new Episode(*this);
    }
    string Episode::toString() const {
        ostringstream strout;
        strout<<to_string(this->get_id())<<". " << this->get_seriesName() << " S"<<to_string(this->get_season()) <<"E"<<to_string(this->get_episode())<<" " <<this->get_length()<<" minutes";
        return strout.str().append(this->concat_tags());
    }
    Watchable* Episode::getNextWatchable(Session &) const {}//TODO:implement
    const string Episode::get_seriesName() const {return this->seriesName;}
    int Episode::get_episode() const {return this->episode;}
    int Episode::get_season() const {return this->season;}
    long Episode::get_next_EpisodeId() { return this->nextEpisodeId;}

;


//endregion

