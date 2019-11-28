#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

class Watchable;
class Session;

class User{
public:
    User(const string& name); //parameter constractor
    User(const User& other);//copy constructor
    User(User&& other);//Move constructor
    virtual User& operator=(const User& other); //copy assignment operator
    virtual User& operator=(User&& other); //move assign operator
    virtual User* clone()=0;
    virtual ~User();//destructor
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual void watch(Watchable* watched_content,Session &sess);
    virtual string toString()=0;
    void setName(string name);
    vector<Watchable*> get_history() const;
    vector<Watchable *> get_remaning_watchable() const;
    void set_remaning_watchable(vector<Watchable*> new_remaning);
    virtual void clear();
    string getName() const;
protected:
    vector<Watchable*> history;
private:
    string name;
    vector<Watchable*> remaning_watchable;
};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const string &name);//parameter constructor
    LengthRecommenderUser(const LengthRecommenderUser& other);//copy constructor
    LengthRecommenderUser(LengthRecommenderUser&& other);//Move constructor
    LengthRecommenderUser& operator=(const LengthRecommenderUser& other); //copy assignment operator
    LengthRecommenderUser& operator=(LengthRecommenderUser&& other); //move assign operator
    virtual Watchable* getRecommendation(Session& s);
    virtual string toString();
    virtual LengthRecommenderUser* clone();
    void calculate_avg_time();

private:
    double avgTime;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    RerunRecommenderUser(RerunRecommenderUser&& other);//Move constructor
    RerunRecommenderUser& operator=(const RerunRecommenderUser& other); //copy assignment operator
    RerunRecommenderUser& operator=(RerunRecommenderUser&& other); //move assign operator
    virtual Watchable* getRecommendation(Session& s);
    virtual string toString();
    virtual RerunRecommenderUser* clone();
    int get_index() const;

private:
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const string& name);
    GenreRecommenderUser(const GenreRecommenderUser& other);
    GenreRecommenderUser(GenreRecommenderUser&& other);//Move constructor
    GenreRecommenderUser& operator=(const GenreRecommenderUser& other); //copy assignment operator
    GenreRecommenderUser& operator=(GenreRecommenderUser&& other); //move assign operator
    ~GenreRecommenderUser();
    virtual void watch(Watchable* watched_content,Session &sess);
    virtual Watchable* getRecommendation(Session& s);
    void add_tag_freq(vector<string> tags);
    void set_remaning_watchable(vector<Watchable*> all_content);
    vector<pair<string,int>> sort_tags();
    virtual GenreRecommenderUser* clone();
    virtual string toString();
    Watchable* get_content_by_tag(string tag);
    virtual void clear();
    map<string, int> get_tags_freq();
private:
    map<string, int>  tags_freq;
};

#endif
