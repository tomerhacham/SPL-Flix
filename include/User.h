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
    virtual User& operator=(const User& other); //copy assignment operator
    virtual User& operator=(User&& other); //move assign operator
    virtual User* clone()=0;
    virtual ~User();//destructor
    virtual Watchable* getRecommendation(Session& s) = 0;
    virtual void watch(Watchable* watched_content);
    virtual string toString()=0;
    void setName(string name);
    vector<Watchable*> get_history() const;
    string getName() const;
protected:
    vector<Watchable*> history;
private:
    string name;
};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const string &name);//parameter constructor
    LengthRecommenderUser(const LengthRecommenderUser& other);//copy constructor
    LengthRecommenderUser(LengthRecommenderUser&& other);//Move constructor
    LengthRecommenderUser& operator=(const LengthRecommenderUser& other); //copy assignment operator
    LengthRecommenderUser& operator=(LengthRecommenderUser&& other); //move assign operator
    virtual ~LengthRecommenderUser();//destructor
    virtual Watchable* getRecommendation(Session& s);
    virtual string toString();
    virtual LengthRecommenderUser* clone();
    vector<Watchable*> get_remaning_watchable();
    void calculate_avg_time();
    friend ostream& operator<<(ostream& os, const LengthRecommenderUser & user);
protected:
    void set_remaning_watchable(vector<Watchable*> all_content);

private:
    double avgTime;
    vector<Watchable*> remaning_watchable;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    RerunRecommenderUser(RerunRecommenderUser&& other);//Move constructor
    RerunRecommenderUser& operator=(const RerunRecommenderUser& other); //copy assignment operator
    RerunRecommenderUser& operator=(RerunRecommenderUser&& other); //move assign operator
    friend ostream& operator<<(ostream& os, const RerunRecommenderUser & user);
    virtual Watchable* getRecommendation(Session& s);
    virtual string toString();
    virtual RerunRecommenderUser* clone();
    int get_index();

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
    virtual Watchable* getRecommendation(Session& s);
    void add_tag_freq(const string& tag);
    void set_remaning_watchable(vector<Watchable*> all_content);
    friend ostream& operator<<(ostream& os, const GenreRecommenderUser & user);
    virtual GenreRecommenderUser* clone();
    virtual string toString();
    vector<Watchable*> get_remaning_watchable();
    unordered_map<string, int> get_tags_freq();
private:
    unordered_map<string, int>  tags_freq;
    vector<Watchable*> remaning_watchable;
};

#endif
