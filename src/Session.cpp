#include <../include/Session.h>
#include <../include/json.hpp>
#include <../include/User.h>
#include <fstream>
#include <iostream>
#include <../include/Watchable.h>
#include <sstream>

using namespace std;
//Constructors
    Session::Session(const string &configFilePath)//parameter constructor
    {
    using json= nlohmann::json;
    ifstream i(configFilePath);
    json inf;
    i >> inf;


    int id=1;
    int length;
    string name;

   for(int i=0; i< inf["movies"].size(); i++){
        length = inf["movies"][i]["length"];
        name= inf["movies"][i]["name"];
        this->content.push_back(new Movie(id, name, length , inf["movies"][i]["tags"]));
        id++;

    }

        for(int k=0; k< inf["tv_series"].size(); k++){
            for(int m=1; m<= inf["tv_series"][k]["seasons"].size(); m++){
                for(int n = 1; n <= inf["tv_series"][k]["seasons"][m-1]; ++n) {
                     length= (inf["tv_series"][k]["episode_length"]);
                     name= inf["tv_series"][k]["name"];
                     if (n == inf["tv_series"][k]["seasons"][m-1]) {
                        content.push_back(new Episode(id, name, length , m ,n, inf["tv_series"][k]["tags"], -1));
                     }
                    else  content.push_back(new Episode(id, name , length , m, n, inf["tv_series"][k]["tags"],n+1));
                        id++;
                 }
              }
        }
    for(int f = 0; f < content.size(); ++f)
        std::cout << content[f]->toString() << '\n';
}
    Session::Session(const Session &other):command(other.command), exit(other.exit)//copy constructor
    {
        for (int i=0; i<other.content.size(); i++){
            Watchable* watch= other.content.at(i);
            this->content.push_back(watch->clone());
        }
        for(int j=0; j< other.actionsLog.size(); j++){
            BaseAction* bsa= other.actionsLog.at(j);
            this->actionsLog.push_back(bsa->clone());
        }
        for(auto pair : other.userMap)
        {
            this->userMap.insert(userMap.begin(),pair);
        }

        this->activeUser= other.activeUser;
        for (int k = 0; k <other.parameters.size() ; ++k) {
            this->parameters.push_back(other.parameters.at(k));
        }
    }
    Session::Session(Session&& other)// move constructor
    {
        this->clear();
        this->content= other.content;
        this->actionsLog= other.actionsLog;
        this->userMap= other.userMap;
        this->activeUser= other.activeUser;
        this->parameters= other.parameters;
        this->command= other.command;
        this->exit= other.exit;
        other.clear();
    }
    Session& Session::operator=(const Session& other)// copy Assigment operator
     {
        if(this!= &other)
        {
            this->clear();
            for (int i = 0; i < other.content.size(); i++) {
                Watchable *watch = other.content.at(i);
                this->content.push_back(watch->clone());
            }
            for (int j = 0; j < other.actionsLog.size(); j++) {
                BaseAction *bsa = other.actionsLog.at(j);
                this->actionsLog.push_back(bsa->clone());
            }
            for(auto pair : other.userMap)
            {
                this->userMap.insert(userMap.begin(),pair);
            }
            this->activeUser = other.activeUser->clone();
            for (int k = 0; k < other.parameters.size(); ++k) {
                this->parameters.push_back(other.parameters.at(k));
            }
            this->command = other.command;
            this->exit = other.exit;
        }
        return *this;
    }
    Session & Session:: operator=(Session && other)//Move assngment operator
     {
        if(this!= &other){
            this->clear();
            this->content= other.content;
            this->actionsLog= other.actionsLog;
            this->userMap= other.userMap;
            this->activeUser= other.activeUser;
            this->parameters= other.parameters;
            this->command= other.command;
            this->exit= other.exit;
            other.clear();
        }
        return *this;
     }
    Session::~Session() //Destructor
    {
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
        while (!exit) {
            cout<<"HERE";
            string input;
            cin >> input;
            parsing(input);

            if (command == "createuser") {
                CreateUser *cu = new CreateUser();
                cu->act(*this);
            }

            if (command == "changeuser") {
                ChangeActiveUser* cat = new ChangeActiveUser();
                cat->act(*this);
            }

            if (command == "deleteuser") {
                DeleteUser* du = new DeleteUser();
                du->act(*this);
            }

            if (command == "dupuser") {
                DuplicateUser* dpu = new DuplicateUser();
                dpu->act(*this);
            }

            if (command == "content") {
                PrintContentList* pcl = new PrintContentList();
                pcl->act(*this);
            }

            if (command == "watchhist") {
                PrintWatchHistory* pwh = new PrintWatchHistory();
                pwh->act(*this);
            }

            if (command == "watch") {
                Watch* wat = new Watch();
                wat->act(*this);
            }

            if (command == "log") {
                PrintActionsLog* pal = new PrintActionsLog();
                pal->act(*this);
            }

            if (command == "exit") {
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
        stringstream ss(s);
        istream_iterator<string> begin(ss);
        istream_iterator<string> end;
        vector<string> parameters(begin, end);
        this->parameters= parameters;
        command= this->parameters.at(0);
        this->parameters.erase(parameters.begin()+0);
    }
    void Session::delete_user(User * user) {
        userMap.erase(user->getName());
        delete user;
    }
    void Session::Exit_session() {
        this->exit=true;
    }
    void Session::clear(){
        for(auto watchable:content){
            delete watchable;
        }
        for(auto action:actionsLog){
            delete action;
        }
        for(auto elem: this->userMap){
            User* user= elem.second;
            delete user;
        }
        this->userMap.clear();
        this->activeUser=nullptr;
        this->parameters.clear();
    }

    vector<string> Session::get_parameters() {
        return this->parameters;
    }

    vector<Watchable *> Session::get_content() {  return this->content;
    }

void Session::accept_recommendation(long recomended_id) {
        this->parameters.at(0)=to_string(recomended_id);
        Watch* RecWatch = new Watch();
        RecWatch->act(*this);

}


//for(int k=0; k< inf["tv_series"].size(); k++){
//for(int m=1; m<= inf["tv_series"][k]["seasons"].size(); m++){
//for(int n = 1; n <= inf["tv_series"][k]["seasons"][m]; ++n) {
//length= (inf["tv_series"][k]["episode_length"]);
//name= inf["tv_series"][k]["name"];
//if (n == inf["tv_series"][k]["seasons"][m]) {
//content.push_back(
//new Episode(id, name, length , m ,n, inf["tv_series"][k]["tags"], -1));
//}
//else  content.push_back(new Episode(id, name , length , m, n, inf["tv_series"][k]["tags"],n+1));
//id++;