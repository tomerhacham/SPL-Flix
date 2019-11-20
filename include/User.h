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
    User(const std::string& name);
    User(const User& other);
    virtual ~User();
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
private:
    double avgTime;
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    RerunRecommenderUser(const RerunRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
private:
    int index;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    GenreRecommenderUser(const GenreRecommenderUser& other);
    virtual Watchable* getRecommendation(Session& s);
private:
    //TODO: add some kind of data structure to count how many of the tags has the user watched
};

#endif
