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
    Session::Session(const Session &other) {}

//Methods:

    public vector<string&> get_parameters(){
    return this.parameters();
    }
    public vector<Watchable*> get_content(){
    return  this.content;
    }
    public vector<string> parsing(string s){

    }
    void Session::start() {}

    //TODO: need to make a getter for the content vector
    void Session::addUser(User* user) {
        userMap.insert(user->getName(), user);
    }
    void Session::addAction(BaseAction* act) {
        actionsLog.push_back(act);
}
    void Session::getUserByName(string key) {
        std::unordered_map<std::string,User*>::iterator iter= userMap.find(key);
        if ( iter == userMap.end() )
            return nullptr;
        else
            return userMap.at(key);
}
    void Session::Change_Active_User(User * user) {
        this->activeUser= nullptr;
        this->activeUser= user;
    }
    void Session::Get_parametrs() {}// check if this watch,createuser.. and put the string into vector and play the right method


