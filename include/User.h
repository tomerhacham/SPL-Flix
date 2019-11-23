#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

class Watchable;
class Session;

class User{
public:
    User(const std::string& name); //parameter constractor
    User(const User& other);//copy constructor
    User(User&& other);//Move constructor
    User& operator=(const User& other); //copy assignment operator
    User& operator=(User&& other); //move assign operator

    virtual ~User();//destructor
    virtual Watchable* getRecommendation(Session& s) = 0;

    std::string getName() const;
    std::vector<Watchable*> get_history() const;
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;

};

class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string &name);
    LengthRecommenderUser(const LengthRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    LengthRecommenderUser(LengthRecommenderUser&& other);//Move constructor
    LengthRecommenderUser& operator=(const LengthRecommenderUser& other); //copy assignment operator
    LengthRecommenderUser& operator=(LengthRecommenderUser&& other); //move assign operator
    friend std::ostream& operator<<(std::ostream& os, const LengthRecommenderUser & user);
protected:
    double calculate_avg_time();
    void set_remaning_watchable(std::vector<Watchable*> remaning_watchable);

private:
    double avgTime;
    std::vector<Watchable*> remaning_watchable;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    friend std::ostream& operator<<(std::ostream& os, const RerunRecommenderUser & user);

private:
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const GenreRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
    void add_tag_freq(const std::string& tag);
    void set_remaning_watchable(std::vector<Watchable*> remaning_watchable);
    friend std::ostream& operator<<(std::ostream& os, const GenreRecommenderUser & user);


private:
    std::unordered_map<std::string, int>  tags_freq; //TODO: add some kind of data structure to count how many of the tags has the user watched
    std::vector<Watchable*> remaning_watchable;
};

#endif
