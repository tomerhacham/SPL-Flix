#include <include/Watchable.h>
#include "../include/User.h"

//region User abstract

    //Constructors
    User::User(const std::string &name): name(name) {}//parameter constructor
    User::User(const User& other): name(other.name)//copy constructor
    {
        for(int i=0;i<other.history.size();i++){
            Watchable* watch = other.history.at(i);
            this->history.push_back(watch->clone());
        }

    }
    User& User::operator=(const User &other) //copy assignment operator
    {
        if(this!=&other){
            for(int i=0;i<history.size();i++){
                Watchable* watch = history.at(i);
                delete watch;
                this->history.at(i)=other.history.at(i)->clone();
            }
        }
        return *this;
    }
    User& User::operator=(User && other)//move assignment operator
    {
        if(this!=&other){
            for(int i=0;i<history.size();i++) {
                Watchable *watch = history.at(i);
                delete watch;
            }
            this->history=other.history;
            //TODO: how to change the pointer of the other's vector that it will not delete mine

        }
    }
    User::~User()//destructor
    {
    for(int i=0;i<history.size();i++){
        Watchable* watch = this->history.at(i);
        delete watch;
     }
    history.clear();
    }

    //Methods:
    std::string User::getName() const {return this->name;}
    std::vector<Watchable*> User::get_history() const {return this->history;}

//endregion

//region User - Length Recommender

    //Constructor:
    LengthRecommenderUser::LengthRecommenderUser(const std::string &name):User(name){}
    LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other):User(other), avgTime(avgTime) {}

    LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser &&other):User::User(other), avgTime(other.avgTime)//Move constructor
    {
    //TODO: do we need to clear the pointer of the vector?
    }
    LengthRecommenderUser& LengthRecommenderUser::operator=(const LengthRecommenderUser &other){}//copy assignment operator
    LengthRecommenderUser& LengthRecommenderUser::operator=(LengthRecommenderUser &&other){}//Move assingment operator

    double LengthRecommenderUser::calculate_avg_time() //calculate the avg time that the user has been watched
    {
        double avg=0;
        for(int i=0;i<history.size();i++){
            avg=avg+history.at(i)->get_length();
        }
        this->avgTime=(avg/history.size());
    }
    void LengthRecommenderUser::set_remaning_watchable(std::vector<Watchable *> remaning_watchable)
    {
        this->remaning_watchable=remaning_watchable;
    }
    Watchable* LengthRecommenderUser::getRecommendation(Session &s) {}//TODO:: implements by the algorithm
//endregion

//region User - Rerun Recommender

    //Constructor
    RerunRecommenderUser::RerunRecommenderUser(const std::string &name): User(name){}
    RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser& other): User(other), index(other.index) {}

    //Methods
    Watchable* RerunRecommenderUser::getRecommendation(Session &s) {}//TODO:: implements by the algorithm
//endregion

//region User - Similar Genre

    // Constructors
    GenreRecommenderUser::GenreRecommenderUser(const std::string &name): User(name){}
    GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser& other): User(other){}

    //Methods
    Watchable* GenreRecommenderUser::getRecommendation(Session &s) {}//TODO: we needs to impelemnt some king of data structure to store the number of the stuff the the user watch per tag
//endregion