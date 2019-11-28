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
	virtual std::string toString() const=0;
    void complete();
    void error(const string& errorMsg);
    string getErrorMsg() const;
    void set_ErrorMsg(string ErrorMsg);
    void set_status(ActionStatus status);

private:
	string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    bool is_valid_algorithm(string parameter);
    virtual void act(Session& sess);
	virtual string toString() const;

};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
};

class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
};

class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual void reWatch(Session& sess, Watchable* recommendation);
	virtual std::string toString() const;
};

class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
};
#endif
