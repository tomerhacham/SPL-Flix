#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <../include/Action.h>

class User;
class Watchable;

class Session{
public:

    Session(const string &configFilePath);//parameter constructor
    Session(const Session& other);//copy constructor
    Session(Session&& other);// move constructor
    virtual Session& operator=(const Session& other);// copy Assigment operator
    virtual Session& operator=(Session&& other);// move assigment operator
    ~Session();//destructor

    public: vector<string> get_parameters();
    public: vector<Watchable*> get_content();
    public: Watchable* find_content_by_id(long);
    public: User* get_active_user();
    public: vector<BaseAction*> get_actionlog();
    public: User* get_userbyName(string);

    void start();
    void addUser(User*);
    void addAction(BaseAction*);
    void change_active_user (User*);
    void parsing(string);
    void delete_user(User*);
    void Exit_session();
    void clear();
    void accept_recommendation(long recomended_id);


private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::vector<string> parameters;
    string command;
    bool exit;
};
#endif
