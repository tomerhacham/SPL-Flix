#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
using namespace std;


class Session;

class Watchable{
public:
    Watchable(long id, int length, const vector<string>& tags);
    Watchable(const Watchable& other);
    virtual ~Watchable();
    virtual string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual Watchable* clone()=0;
    const long get_id() const;
    const int get_length() const;
    const vector<string> get_tags() const;
    const string concat_tags() const;

private:
    const long id;
    int length;
    vector<string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const vector<string>& tags);
    Movie(const Movie& other);
    virtual ~ Movie();
    virtual string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Movie* clone();
    const string get_name();
private:
    string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    Episode(const Episode& other);
    virtual ~Episode();
    virtual string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual Episode* clone();
    const string get_seriesName() const;
    int get_season() const;
    int get_episode() const;
    long get_next_EpisodeId();


private:
    string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
