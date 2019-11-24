#include <iostream>
#include <vector>
#include <include/User.h>
#include <sstream>
#include <include/Session.h>
#include "include/Action.h"
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
            session.addUser(new_user);//TODO: implement at Session
            this->complete();
        }
        else{
            this->error("Invalid Algorithem");
        }
        session.addAction(this);//TODO: implemnt at Session

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
        vector<string> parameters = session.get_parameters(); //TODO:implement get_param method
        string username = parameters.at(0);
        User* user = session.get_userbyName(username);//TODO: implement get_userByname at session return nullptr if not found
        if(user!=nullptr) {
            session.change_active_user(user);//TODO: implement change_active_user at session
            this->complete();
        }
        else{
            this->error("Error at ChangeActiveUser");
        }
        session.addAction(this);//TODO: implemnt at Session

    }
    std::string ChangeActiveUser::toString() const {}
//endregion

//region DeleteUser

    //Methods:
    void DeleteUser::act(Session &sess) {
        Session& session = sess;
        vector<string> parameters = session.get_parameters(); //TODO:implement get_param method
        string username = parameters.at(0);
        User* user = session.get_userbyName(username);//TODO: implement get_userByname at session return nullptr if not found
        if(user!= nullptr){
            session.delete_user(user);
            this->complete();
        }
        else{
            this->error("Error at Delete User");
        }
        session.addAction(this);//TODO: implemnt at Session
    }
    std::string DeleteUser::toString() const {}
//endregion

//region DuplicateUser

    //Methods:
    void DuplicateUser::act(Session &sess)
    {
        Session& session = sess;
        vector<string> parameters = session.get_parameters(); //TODO:implement get_param method
        string username = parameters.at(0);
        string new_username = parameters.at(1);
        User* user = session.get_userbyName(username);//TODO: implement get_userByname at session return nullptr if not found
        if(username==new_username){
            this->error("Duplicate user with the same Username");
        }
        else if(user!= nullptr){
            User* duplicate_user =user->clone();
            session.addUser(duplicate_user);
            this->complete();
        }
    }
    std::string PrintContentList::toString() const {}
//endregion

// region PrintWatchHistory
    //Constructors:

    //Methods:
    void PrintWatchHistory::act(Session &sess) {
        Session& session = sess;



    }
    string PrintWatchHistory::toString() const {}
//endregion

// region Watch
    //Constructors:

    //Methods:
    void Watch::act(Session &sess) {}
    std::string Watch::toString() const {}
//endregion

// region PrintActionsLog
    //Constructors:

    //Methods:
    void PrintActionsLog::act(Session &sess) {}
    std::string PrintActionsLog::toString() const {}
//endregion

// region Exit
    //Constructors:

    //Methods:
    void Exit::act(Session &sess) {}
    std::string Exit::toString() const {}
//endregion
