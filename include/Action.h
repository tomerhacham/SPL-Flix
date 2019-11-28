#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
using namespace std;

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual string toString() const=0;
    void complete();
    virtual BaseAction* clone()=0;
    void error(const string& errorMsg);
    string getErrorMsg() const;
    void set_ErrorMsg(string ErrorMsg);
    void set_status(ActionStatus status);
    string StatusToString(ActionStatus status) const;

private:
	string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    bool is_valid_algorithm(string parameter);
    virtual void act(Session& sess);
	virtual string toString() const;
	CreateUser* clone();

};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual string toString() const;
    ChangeActiveUser* clone();

};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual string toString() const;
    DeleteUser* clone();
};

class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual string toString() const;
    DuplicateUser* clone();

};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual string toString() const;
    PrintContentList* clone();

};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual string toString() const;
    PrintWatchHistory* clone();

};

class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual string toString() const;
    Watch* clone();

};

class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual string toString() const;
    PrintActionsLog* clone();

};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual string toString() const;
    Exit* clone();

};
#endif
