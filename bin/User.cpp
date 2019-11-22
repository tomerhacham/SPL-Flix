#include "../include/User.h"endregin

//region User abstract

    //Constructors
    User::User(const std::string &name): name(name) {}
    User::User(const User& other): name(other.name)
    {
        for(int i=0;i<other.history.size();i++){
            this->history.push_back(other.history[i]);
        }

    }
    User::~User()
    {
        for(int i=0;i<history.size();i++){
           delete  history[i];
        }

    }

    //Methods
    std::vector<Watchable*> User::get_history() const
    {
        return this->history;
    }
    std::string User::getName() const
    {
        return this->name;
    }
    //endregion

//region User - Length Recommender

    //Constructors
    LengthRecommenderUser::LengthRecommenderUser(const std::string &name): User(name) {}
    LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser& other): User(other), avgTime(other.avgTime){}

    //Methods
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