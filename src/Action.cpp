#include <iostream>
#include <vector>
#include <../include/User.h>
#include <sstream>
#include <../include/Session.h>
#include <../include/Action.h>
#include <../include/Watchable.h>
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
    string BaseAction::StatusToString(ActionStatus status) const {
            switch (status)
            {
                case COMPLETED:   return "COMPLETE";
                case ERROR:   return "ERROR:";
                case PENDING: return "PENDING";
            }
        }
//endregion

//region CreateUser

    //Methods:
    void CreateUser::act(Session &sess) {
        Session* session = &sess;
        vector<string> parameters = session->get_parameters();
        string username = parameters.at(0);
        string algorithm = parameters.at(1);
        if(session->get_userbyName(username)!=nullptr)
        {
            this->error("the new user name is already taken");
        }
        if(is_valid_algorithm(algorithm) && this->getStatus()!=ERROR){
            User* new_user;
            if (algorithm == "len")
            {
                new_user = new LengthRecommenderUser(username);
                new_user->set_remaning_watchable(session->get_content());
            }
            else if (algorithm == "rer")
            {
                new_user = new RerunRecommenderUser(username);
            }
            else if (algorithm == "gen")
            {
                new_user = new GenreRecommenderUser(username);
                new_user->set_remaning_watchable(session->get_content());
            }
            session->addUser(new_user);
            session->change_active_user(new_user);
            this->complete();
        }
        else{this->error("Invalid Algorithem");}
        session->addAction(this);
        return;

    }
        string CreateUser::toString() const
        {
            string toReturn="CreateUser ";
            toReturn.append(StatusToString(this->getStatus()));
            toReturn.append(" ");
            toReturn.append(this->getErrorMsg());
            return toReturn;
        }
    bool CreateUser::is_valid_algorithm(string parameter) {
        if(parameter!="rer" && parameter!="len" && parameter!="gen"){
            return false;
        }
        return true;
}

    CreateUser *CreateUser::clone() {
        return new CreateUser(*this);
    }

//endregion

//region ChangeActiveUser

    //Methods:
    void ChangeActiveUser::act(Session &sess)
    {
        Session& session = sess;
        vector<string> parameters = session.get_parameters();
        string username = parameters.at(0);
        User* user = session.get_userbyName(username);
        if(user!=nullptr) {
            session.change_active_user(user);
            this->complete();
        }
        else{
            this->error("Error at ChangeActiveUser");
        }
        session.addAction(this);

    }
    string ChangeActiveUser::toString() const {
        string toReturn="ChangeActiveUser ";
        toReturn.append(StatusToString(this->getStatus()));
        toReturn.append(" ");
        toReturn.append(this->getErrorMsg());
        return toReturn;

    }

    ChangeActiveUser *ChangeActiveUser::clone() {
        return new ChangeActiveUser(*this);

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
        string toReturn="DeleteUser ";
        toReturn.append(StatusToString(this->getStatus()));
        toReturn.append(" ");
        toReturn.append(this->getErrorMsg());
        return toReturn;

    }

    DeleteUser *DeleteUser::clone() {
        return new DeleteUser(*this);
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
        if(user== nullptr)
            {this->error("User not found!");}
        else if(user!= nullptr && this->getStatus()!=ERROR){
            User* duplicate_user =user->clone();
            duplicate_user->setName(new_username);
            session->addUser(duplicate_user);
            this->complete();
        }
    }
    string DuplicateUser::toString() const {
        string toReturn="DuplicateUser ";
        toReturn.append(StatusToString(this->getStatus()));
        toReturn.append(" ");
        toReturn.append(this->getErrorMsg());
        return toReturn;
    }

DuplicateUser *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}
//endregion

// region PrintContentList
    //Constructors:

    //Methods:
    void PrintContentList::act(Session &sess) {
        Session* session = &sess;

        vector<Watchable *> content = session->get_content();
        if(!content.empty()){
        for(int i=0; i<content.size();i++){
            string s = content.at(i)->toString();
            cout<<to_string(i+1).append(". " )<< s<<endl;
        }
        this->complete();
    }
        else{
            this->error("Content is Empty");
        }
    }
    string PrintContentList::toString() const {
            string toReturn="PrintContentList ";
            toReturn.append(StatusToString(this->getStatus()));
            toReturn.append(" ");
            toReturn.append(this->getErrorMsg());
            return toReturn;
    }

PrintContentList *PrintContentList::clone() {
    return new PrintContentList(*this);
}
//endregion

// region Watch

    //Methods:
    void Watch::act(Session &sess)
    {
        bool accept_rec=false;
        Session* session = &sess;
        session->addAction(this);
        stringstream stm(session->get_parameters().at(0));
        long content_id=0;
        stm>>content_id; //preform cast of string to int in order to search the content_id
        Watchable* toWatch= session->find_content_by_id(content_id);
        Watchable* recom=nullptr;
        if(toWatch!=nullptr) {
            User* activeuser = session->get_active_user();
            activeuser->watch(toWatch,sess);
            this->complete();
            recom = toWatch->getNextWatchable(sess);
            cout<<"We recommend watching "<<recom->toString()<<", continue watching? [y/n]"<<endl;
            string input;
            getline(cin,input);
            cout<<input<<endl;
            if(input=="y")
            {accept_rec=true;}
        }
        else{
            this->error("Content ID did not found");
        }
        if(accept_rec){session->accept_recommendation(recom->get_id());}
    }
    string Watch::toString() const {
        string toReturn="Watch ";
        toReturn.append(StatusToString(this->getStatus()));
        toReturn.append(" ");
        toReturn.append(this->getErrorMsg());
        return toReturn;

    }

Watch *Watch::clone() {
    return new Watch(*this);
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
        string toReturn="PrintActionsLog ";
        toReturn.append(StatusToString(this->getStatus()));
        toReturn.append(" ");
        toReturn.append(this->getErrorMsg());
        return toReturn;
    }

PrintActionsLog *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}
//endregion

// region Exit

    //Methods:
    void Exit::act(Session &sess)
    {
        Session* session = &sess;
        session->Exit_session();

    }
    string Exit::toString() const {
        string toReturn="Exit";
        toReturn.append((const char*) this->getStatus());
        toReturn.append(this->getErrorMsg());
        return toReturn;
    }

Exit *Exit::clone() {
    return new Exit(*this);
}
//endregion

//region PrintWatchHistory
void PrintWatchHistory::act(Session &sess) {
    Session* session = &sess;
    vector<Watchable *> content = session->get_active_user()->get_history();
    if(!content.empty()){
        cout<<"Watch history for "<<session->get_active_user()->getName()<<endl;
        for(int i=0; i<content.size();i++){
            cout<<content.at(i)->toString()<<endl;
            this->complete();
        }
    }
    else{
        this->error("History is empty");
    }
}
string PrintWatchHistory::toString() const {
    string toReturn="PrintWatchHistory ";
    toReturn.append(StatusToString(this->getStatus()));
    toReturn.append(" ");
    toReturn.append(this->getErrorMsg());
    return toReturn;
}

PrintWatchHistory *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}
//endregion

