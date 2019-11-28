#include "include/Session.h"
#include <../include/json.hpp>
#include <include/User.h>
#include <fstream>
#include <iostream>
#include <Watchable.h>
#include <sstream>

using namespace std;
//Constructors
    Session::Session(const string &configFilePath) {
    using json= nlohmann::json;
    ifstream i("/home/tomer/SPL/assignment_1/config1.json");
    json inf;
    i >> inf;


    int id=1;
    for(int i=0; i<inf["movies"].size(); i++){
        content.push_back(new Movie(id, inf["movies"][i]["name"], inf["movies"][i]["length"], inf["movies"][i]["tags"]));
        id++;
    }

    for(int k=0; k< inf["tv_series"].size(); k++){
        for(int season=1; season<= inf["tv_series"][k]["seasons"].size(); season++){
            for(int episode = 1; episode <= inf["tv_series"][k]["seasons"][season-1]; ++episode) {
                if (episode == inf["tv_series"][k]["seasons"][season-1]) {
                    content.push_back(new Episode(id, inf["tv_series"][k]["name"], inf["tv_series"][k]["episode_length"], season,episode, inf["tv_series"][k]["tags"], -1));
                }
                else  {
                    Episode* e = new Episode(id, inf["tv_series"][k]["name"], inf["tv_series"][k]["episode_length"], season, episode, inf["tv_series"][k]["tags"],episode+1);
                    content.push_back(e);
                }
                id++;
            }
        }
    }
}//TODO: to check if it works
    Session::Session(const Session &other) {

    }//TODO: implement this copy constructor
    Session::~Session() {
        delete this->activeUser;
        for(auto elem: this->userMap){
            User* user= elem.second;
            delete user;
        }
        this->userMap.clear();

        for(int i=0;i<this->content.size();i++) {
            Watchable* cont = this->content.at(i);
            delete cont;
        }
        this->content.clear();
        for(int i=0;i<this->actionsLog.size();i++) {
            BaseAction* act = this->actionsLog.at(i);
            delete act;
        }
        this->actionsLog.clear();
        this->parameters.clear();
    }


//Methods:


    Watchable * Session::find_content_by_id(long id) {
        for(int i=0; i<content.size(); i++){
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
    User* Session::get_userbyName(string key) {
        unordered_map<string,User*>::iterator iter= userMap.find(key);
        if ( iter == userMap.end() )
            return nullptr;
        else
            return userMap.at(key);
    }

    void Session::start() {
        exit = false;
        cout << "SPLFLIX is now on!”" << endl;
        LengthRecommenderUser* def = new LengthRecommenderUser("default");
        addUser(def);
        activeUser=def;
        while (!exit) {
            string input;
            getline(cin,input);
            cout<<input<<endl;
            parsing(input);
            for(int i=0;i<parameters.size();i++){cout<<parameters[i]<<endl;}

            if (command == "createuser") {
                CreateUser *cu = new CreateUser();
                cu->act(*this);
            }

            else if (command == "changeuser") {
                ChangeActiveUser* cat = new ChangeActiveUser();
                cat->act(*this);
            }

            else if (command == "deleteuser") {
                DeleteUser* du = new DeleteUser();
                du->act(*this);
            }

            else if (command == "dupuser") {
                DuplicateUser* dpu = new DuplicateUser();
                dpu->act(*this);
            }

            else if (command == "content") {
                PrintContentList* pcl = new PrintContentList();
                pcl->act(*this);
            }

            else if (command == "watchhist") {
                PrintWatchHistory* pwh = new PrintWatchHistory();
                pwh->act(*this);
            }

            else if (command == "watch") {
                Watch* wat = new Watch();
                wat->act(*this);
            }

            else if (command == "log") {
                PrintActionsLog* pal = new PrintActionsLog();
                pal->act(*this);
            }

            else if (command == "exit") {
                Exit* ext = new Exit();
                ext->act(*this);
            }
            }
        }

    void Session::addUser(User* user) {
        userMap.insert(pair<string,User*>(user->getName(),user));

    }
    void Session::addAction(BaseAction* act) {
        actionsLog.push_back(act);
}
    void Session::change_active_user(User * user) {
        this->activeUser= nullptr;
        this->activeUser= user;
    }
    void Session::parsing(string s)  {
        istringstream iss(s);
        vector<string> param ((istream_iterator<string>(iss)),istream_iterator<string>());
        this->parameters.clear();
        command=param.front();
        param.erase(param.begin()+0);
        parameters=param;
        /*stringstream ss(s);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> parameters(begin, end);
        this->parameters= parameters;
        command= this->parameters.at(0);
        this->parameters.erase(parameters.begin()+0);*/
    }
    void Session::delete_user(User * user) {
        userMap.erase(user->getName());
        delete user;
    }
    void Session::Exit_session() {
        this->exit=true;
    }

    vector<string> Session::get_parameters() {
        return this->parameters;
    }

    vector<Watchable *> Session::get_content() {  return this->content;
    }

void Session::accept_recommendation(Watchable * recomendation) {
        Watch* watchrec = new Watch();
        watchrec->act(*this);

}


