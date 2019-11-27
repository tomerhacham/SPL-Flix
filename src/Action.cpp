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
    BaseAction::BaseAction() :status(PENDING), errorMsg(""){}

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
        vector<string> parameters = session.get_parameters();
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
            this->error("Invalid Algorithm");
        }
        session.addAction(this);

    }
        string CreateUser::toString() const
        {
            string toReturn="CreateUser";
            toReturn.append((const char*) this->getStatus());
            toReturn.append(this->getErrorMsg());
            return toReturn;
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
    string ChangeActiveUser::toString() const {
        string toReturn="ChangeActiveUser";
        toReturn.append((const char*) this->getStatus());
        toReturn.append(this->getErrorMsg());
        return toReturn;

    }
//endregion

//region DeleteUser

    //Methods:
    void DeleteUser::act(Session &sess) {
        Session* session = &sess;
        vector<string> parameters = session->get_parameters();
        string username = parameters.at(0);
        User* user = session->get_userbyName(username);
        if(user!= nullptr){
            session->delete_user(user);
            this->complete();
        }
        else{
            this->error("Error at Delete User");
        }
        session->addAction(this);
    }
    string DeleteUser::toString() const {
        string toReturn="DeleteUser";
        toReturn.append((const char*) this->getStatus());
        toReturn.append(this->getErrorMsg());
        return toReturn;

    }
//endregion

//region DuplicateUser

    //Methods:
    void DuplicateUser::act(Session &sess)
    {
        Session* session = &sess;
        vector<string> parameters = session->get_parameters();
        string username = parameters.at(0);
        string new_username = parameters.at(1);
        User* user = session->get_userbyName(username);
        if(username==new_username){
            this->error("Duplicate user with the same username");
        }
        else if(user!= nullptr){
            User* duplicate_user =user->clone();
            duplicate_user->setName(new_username);
            session->addUser(duplicate_user);
            this->complete();
        }
    }
    string DuplicateUser::toString() const {
        string toReturn="DuplicateUser";
        toReturn.append((const char*) this->getStatus());
        toReturn.append(this->getErrorMsg());
        return toReturn;
    }
//endregion

// region PrintWatchHistory
    //Constructors:

    //Methods:
    void PrintWatchHistory::act(Session &sess) {
        Session* session = &sess;

        vector<Watchable *> content = session->get_content();
        for(int i=0; i<content.size();i++){
            cout<<content.at(i)->toString();
        }
    }
    string PrintWatchHistory::toString() const {
            string toReturn="PrintWatchHistory";
            toReturn.append((const char*) this->getStatus());
            toReturn.append(this->getErrorMsg());
            return toReturn;
    }
//endregion

// region Watch

    //Methods:
    void Watch::act(Session &sess)
    {
        Session* session = &sess;
        stringstream stm(session->get_parameters().at(0));
        long content_id=0;
        stm>>content_id; //preform cast of string to int in order to search the content_id
        Watchable* toWatch= session->find_content_by_id(content_id);
        if(toWatch!=nullptr) {
            User* activeuser = session->get_active_user();
            activeuser->watch(toWatch,sess);
            this->complete();
        }
        else{
            this->error("Content ID did not found");
        }
        session->addAction(this);
    }
    string Watch::toString() const {
        string toReturn="Watch ";
        toReturn.append((const char*) this->getStatus());
        toReturn.append(" ");
        toReturn.append(this->getErrorMsg());
        return toReturn;

    }
//endregion

// region PrintActionsLog

    //Methods:
    void PrintActionsLog::act(Session &sess) {
        Session* session = &sess;
        vector<BaseAction*> all_action = session->get_actionlog();
        if(!all_action.empty()) {
            for (int i = all_action.size() - 1; i >= 0; i--) {
                cout<<all_action.at(i)->toString()<<"\n"<<endl;
            }
            this->complete();
        }
        else{this->error("Action Log is empty");}
        session->addAction(this);
    }
    string PrintActionsLog::toString() const {
        string toReturn="PrintActionsLog";
        toReturn.append((const char*) this->getStatus());
        toReturn.append(this->getErrorMsg());
        return toReturn;
    }
//endregion

// region Exit
    //Constructors:

    //Methods:
    void Exit::act(Session &sess) {}
    std::string Exit::toString() const {}
//endregion
