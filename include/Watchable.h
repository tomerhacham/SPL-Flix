#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
using namespace std;


class Session;

class Watchable{
public:
    Watchable(long id, int length, const vector<string>& tags);//parameter constructor
    Watchable(const Watchable& other);//copy constructor
    Watchable(Watchable&& other);//Move constructor
    ~Watchable();
    virtual string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual Watchable* clone()=0;
    long get_id() const;
    int get_length() const ;
    vector<string> get_tags() const ;
    string concat_tags() const ;

private:
    const long id;
    int length;
    vector<string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const string& name, int length, const vector<string>& tags);//parameter constructor
    Movie(const Movie& other);//copy constructor
    Movie(Movie&& other);//Move constructor
    virtual string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Movie* clone();
    string get_name();
private:
    string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const string& seriesName,int length, int season, int episode ,const vector<string>& tags, long nextEpisodeId);//parameter constructor
    Episode(const Episode& other);//copy constructor
    Episode(Episode&& other);//Move constructor
    virtual string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Episode* clone();
    const string get_seriesName() const;
    int get_season() const;
    int get_episode() const;
    long get_next_EpisodeId();
    void set_nextID(long nextID);

private:
    string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
