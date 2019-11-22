#include <iostream>
#include "include/Action.h"

//region BaseAction - abstract

    //Constructors:
    BaseAction::BaseAction() :status(PENDING){}

    //Methods:
    void BaseAction::complete()
    {
        this->status=COMPLETED;
    }
    void BaseAction::error(const std::string &errorMsg)
    {
        this->status=ERROR;
        this->errorMsg=errorMsg;
    }
    ActionStatus BaseAction::getStatus() const
    {
    return this->status;
    }
    std::string BaseAction::getErrorMsg() const
    {
        return this->errorMsg;
    }
//endregion

//region CreateUser

    //Constructors:

    //Methods:
    void CreateUser::act(Session &sess) {}
    std::string CreateUser::toString() const {}


//endregion

//region ChangeActiveUser

    //Cunstructors:

    //Methods::
    void ChangeActiveUser::act(Session &sess) {}
    std::string ChangeActiveUser::toString() const {}
//endregion

//region DeleteUser
    //Constructors:

    //Methods:
    void DeleteUser::act(Session &sess) {}
    std::string DeleteUser::toString() const {}
//endregion

//region PrintContentList
    //Constructors:

    //Methods:
    void PrintContentList::act(Session &sess) {}
    std::string PrintContentList::toString() const {}
//endregion

// region PrintWatchHistory
    //Constructors:

    //Methods:
    void PrintWatchHistory::act(Session &sess) {}
    std::string PrintWatchHistory::toString() const {}
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
