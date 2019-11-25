#include <iostream>
#include <vector>
#include <include/User.h>
#include <sstream>
#include <include/Session.h>
#include "include/Action.h"
#include "include/Watchable.h"
using namespace std;

//region BaseAction - abstract

    //Constructors:
    BaseAction::BaseAction() :status(PENDING){}

    //Methods:
    void BaseAction::complete()
    {
        this->status=COMPLETED;
    }
    void BaseAction::error(const string &errorMsg)
    {
        this->status=ERROR;
        this->errorMsg=errorMsg;
    }
    ActionStatus BaseAction::getStatus() const
    {
    return this->status;
    }
    void BaseAction::set_status(ActionStatus status) {this->status=status;}
    std::string BaseAction::getErrorMsg() const
    {
        return this->errorMsg;
    }
    void BaseAction::set_ErrorMsg(string ErrorMsg) {
        this->errorMsg=ErrorMsg;
    }
//endregion

//region CreateUser

    //Methods:
    void CreateUser::act(Session &sess) {
        Session &session = sess;
        vector<string>& parameters = session.get_parameters(); //TODO:implement get_param method
        string username = parameters.at(0);
        string algorithm = parameters.at(1);//TODO: needs to check if there is another user with this name
        if(is_valid_algorithm(algorithm)){
            User* new_user;
            if (algorithm == "len") {
                new_user = new LengthRecommenderUser(username);
            } else if (algorithm == "rer") {
                new_user = new RerunRecommenderUser(username);
            } else if (algorithm == "gen") {
                new_user = new GenreRecommenderUser(username);
            }
            session.addUser(new_user);
            this->complete();
        }
        else{
            this->error("Invalid Algorithem");
        }
        session.addAction(this);

    }
        string CreateUser::toString() const
        {
            ostringstream strout;
            //strout<< ;
            return strout.str();
        }
    bool CreateUser::is_valid_algorithm(string parameter) {
        if(parameter!="rer" && parameter!="len" && parameter!="gen"){
            return false;
        }
        return true;
}

//endregion

//region ChangeActiveUser

    //Methods:
    void ChangeActiveUser::act(Session &sess)
    {
        Session& session = sess;
        vector<string> parameters = session.get_parameters();
        string username = parameters.at(0);
        User* user = session.get_userbyName(username);//TODO: implement get_userByname at session return nullptr if not found
        if(user!=nullptr) {
            session.change_active_user(user);//TODO: implement change_active_user at session
            this->complete();
        }
        else{
            this->error("Error at ChangeActiveUser");
        }
        session.addAction(this);

    }
    std::string ChangeActiveUser::toString() const {}
//endregion

//region DeleteUser

    //Methods:
    void DeleteUser::act(Session &sess) {
        Session& session = sess;
        vector<string> parameters = session.get_parameters();
        string username = parameters.at(0);
        User* user = session.get_userbyName(username);
        if(user!= nullptr){
            session.delete_user(user);//TODO: implement delete_user at session
            this->complete();
        }
        else{
            this->error("Error at Delete User");
        }
        session.addAction(this);
    }
    std::string DeleteUser::toString() const {}
//endregion

//region DuplicateUser

    //Methods:
    void DuplicateUser::act(Session &sess)
    {
        Session& session = sess;
        vector<string> parameters = session.get_parameters();
        string username = parameters.at(0);
        string new_username = parameters.at(1);
        User* user = session.get_userbyName(username);
        if(username==new_username){
            this->error("Duplicate user with the same Username");
        }
        else if(user!= nullptr){
            User* duplicate_user =user->clone();
            duplicate_user->setName(new_username);
            session.addUser(duplicate_user);
            this->complete();
        }
    }
    string PrintContentList::toString() const {}
//endregion

// region PrintWatchHistory
    //Constructors:

    //Methods:
    void PrintWatchHistory::act(Session &sess) {
        Session& session = sess;

        vector<Watchable *> content = session.get_content();
        for(int i=0; i<content.size();i++){
            cout<<content.at(i)->toString();
        }
    }
    string PrintWatchHistory::toString() const {}
//endregion

// region Watch

    //Methods:
    void Watch::act(Session &sess)
    {
        Session& session = sess;
        stringstream stm(session.get_parameters().at(1));
        int content_id=0;
        stm>>content_id; //preform cast of string to int in order to search the content_id
        Watchable* toWatch= session.find_content_by_id();//TODO:implement find_content_by_id at session
        if(toWatch!=nullptr) {
            session.get_active_user().watch //TODO:implement get_active_user at session
            this->complete();
        }
        else{
            this->error("Content ID did not found");
        }
        session.addAction(this);
    }
    string Watch::toString() const {}
//endregion

// region PrintActionsLog

    //Methods:
    void PrintActionsLog::act(Session &sess) {
        Session& session = sess;
        vector<BaseAction*> all_action = session.get_actionlog();//TODO:implement getter for the action log
        if(!all_action.empty()) {
            for (int i = all_action.size() - 1; i >= 0; i--) {
                ostringstream strout;
                ActionStatus status = all_action.at(i)->getStatus();
                strout << all_action.at(i)->toString() << " " << status;
                if(status==ERROR){
                    strout<<": "<<all_action.at(i)->getErrorMsg();
                }
                cout<<strout.str()<<endl;
            }
            this->complete();
        }
        else{this->error("Action Log is empty");}
        session.addAction(this);
    }
    string PrintActionsLog::toString() const {}
//endregion

// region Exit
    //Constructors:

    //Methods:
    void Exit::act(Session &sess) {}
    std::string Exit::toString() const {}
//endregion
