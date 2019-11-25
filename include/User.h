#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Watchable;
class Session;

class User{
public:
    User(const string& name); //parameter constractor
    User(const User& other);//copy constructor
    User(User&& other);//Move constructor
    User& operator=(const User& other); //copy assignment operator
    User& operator=(User&& other); //move assign operator
    virtual User* clone()=0;
    virtual ~User();//destructor
    virtual Watchable* getRecommendation(Session& s) = 0;
    string getName() const;
    void setName(string name);
    virtual void watch(Watchable* watched_content);
    vector<Watchable*> get_history() const;
protected:
    vector<Watchable*> history;
private:
    string name;

};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const string &name);
    LengthRecommenderUser(const LengthRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    LengthRecommenderUser(LengthRecommenderUser&& other);//Move constructor
    LengthRecommenderUser& operator=(const LengthRecommenderUser& other); //copy assignment operator
    LengthRecommenderUser& operator=(LengthRecommenderUser&& other); //move assign operator
    friend ostream& operator<<(ostream& os, const LengthRecommenderUser & user);
    virtual LengthRecommenderUser* clone();
protected:
    double calculate_avg_time();
    void set_remaning_watchable(vector<Watchable*> all_content);

private:
    double avgTime;
    vector<Watchable*> remaning_watchable;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    friend ostream& operator<<(ostream& os, const RerunRecommenderUser & user);
    virtual RerunRecommenderUser* clone();
private:
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const string& name);
    GenreRecommenderUser(const GenreRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    void add_tag_freq(const string& tag);
    void set_remaning_watchable(vector<Watchable*> all_content);
    friend ostream& operator<<(ostream& os, const GenreRecommenderUser & user);
    virtual GenreRecommenderUser* clone();
private:
    unordered_map<string, int>  tags_freq;
    vector<Watchable*> remaning_watchable;
};

#endif
