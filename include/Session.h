#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:

    Session(const std::string &configFilePath);
    Session(const Session& other);
    ~Session();
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
    void accept_recommendation(long recommended_id);
    void parsing(string);
    void delete_user(User*);
    void Exit_session();

private:
    vector<Watchable*> content;
    vector<BaseAction*> actionsLog;
    unordered_map<std::string,User*> userMap;
    User* activeUser;
    vector<string> parameters;
    string command;
    bool exit;
};
#endif
