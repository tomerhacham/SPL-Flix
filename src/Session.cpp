#include "include/Session.h"
#include <../include/json.hpp>
#include <fstream>
#include <iostream>
#include <Watchable.h>
using namespace std;
//Constructors
    Session::Session(const string &configFilePath) {
    using json= nlohamann::json;
    ifstream i(configFilePath);
    json* inf;
    i >> inf;
    cout << inf << endl;

    int id=0;
    for(int i=0; inf["movies"].size(); i++){
        content.push_back(new Movie(id, inf["movies"][i], inf["movies"][i]["name"], inf["movies"][i]["length"], inf["movies"][i]["tags"]))
        id++;
    }

    for(int k=0; k< inf["tv_series"].size(); j++){
        for(int season=1; season<= inf["tv_series"][k]["season"].size(); season++){
            for(int episode = 1; episode <= data["tv_series"][i]["seasons"][season-1]; ++episode) {
                content.push_back(new Episode(id, inf["tv_series"][k]["name"], inf["tv_series"][k]["episode_length"], season, episode, inf["tv_series"][k]["tags"]);
                id++
            }
        }
    }

}
    Session::Session(const Session &other) {

    }//TODO: implement this copy constructor
    Session::~Session() {
        delete this->activeUser;
        this->userMap.clear();
        for(int i=0;i<this->content.size();i++) {
            Watchable *cont = this->content.at(i);
            delete cont;
        }
        this->content.clear();
        for(int i=0;i<this->actionsLog.size();i++) {
            BaseAction* act = this->actionsLog.at(i);
            delete act;
        }
        this->actionsLog.clear();
        for(int i=0;i<this->parameters.size();i++) {
            string parm = this->parameters.at(i);
            delete parm;
        }
        this->parameters.clear();

    }


//Methods:
public:
    vector<string&> get_parameters(){
    return this.parameters();
    }
    vector<Watchable *> Session::get_content(){
    return  this.content;
    }
    Watchable * Session::find_content_by_id(long id) {
        for(i=0; i<content.size(); i++){
         Watchable* w = content.at(i);
            if(w->get_id()== id){
                return w;
            }
        }
      return nullptr;
    }
    User *Session::get_active_user() {
        return this->activeUser;
    }
    vector<BaseAction *> Session::get_actionlog() {
     return this->actionsLog;
    }

    void Session::start() {}
    void Session::addUser(User* user) {
        userMap.insert(user->getName(), user);
    }
    void Session::addAction(BaseAction* act) {
        actionsLog.push_back(act);
}
    void Session::get_userbyName(string key) {
        std::unordered_map<std::string,User*>::iterator iter= userMap.find(key);
        if ( iter == userMap.end() )
            return nullptr;
        else
            return userMap.at(key);
}
    void Session::change_active_user(User * user) {
        this->activeUser= nullptr;
        this->activeUser= user;
    }
    void Session::parsing(string s)  {
        stringstream ss(s);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> parameters(begin, end);
        copy(parameters.begin(), parameters.end(), ostream_iterator<string>);
        this->parameters= parameters;
    }
    void Session::delete_user(User * user) {
        userMap.erase(user);
        delete user;
    }


