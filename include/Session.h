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
    public: Watchable* find_content_by_id(long):
    public: User* get_active_user();
    public: vector<BaseAction*> get_actionlog();

    void start();
    void addUser(User*);
    void addAction(BaseAction*);
    void get_userbyName(string);
    void change_active_user (User*);
    void parsing(string);
    void delete_user(User*);

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    std::vector<string> parameters;
};
#endif
