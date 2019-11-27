#include <include/Watchable.h>
#include "include/User.h"
#include <cmath>        // std::abs
#include <functional>
#include <iostream>

using namespace std;

//region User abstract

    //Constructors
    User::User(const string &name): name(name) {}//parameter constructor
    User::User(const User& other): name(other.getName())//copy constructor
    {
        for(int i=0;i<other.history.size();i++){
            Watchable* watch = other.history.at(i);
            this->history.push_back(watch);
        }
        for(int i=0;i<other.remaning_watchable.size();i++){
            Watchable* watch = other.remaning_watchable.at(i);
            this->remaning_watchable.push_back(watch);
        }
    }
    User::User(User &&other):name(other.getName()) //Move constructor
    {
        for(auto watchable:other.history){
            history.push_back(watchable);
        }
        for(auto watchable:other.get_remaning_watchable()){
            remaning_watchable.push_back(watchable);
        }

        other.clear();
    }
    User& User::operator=(const User &other) //copy assignment operator
    {
        if(this!=&other){
            this->name=other.getName();
            this->clear();
            for(auto watchable : other.history){
                history.push_back(watchable);
            }
            for(auto watchable : other.get_remaning_watchable()){
                remaning_watchable.push_back(watchable);

            }
        }
        return *this;
    }
    User& User::operator=(User && other)//move assignment operator
    {
        if(this!=&other){
            this->clear();
            this->history=other.history;
            this->remaning_watchable=other.remaning_watchable;
            other.clear();
        }
        return *this;
    }
    User::~User()//destructor
    {
        this->clear();
    }

    //Methods:
    string User::getName() const {return this->name;}
    vector<Watchable*> User::get_history() const {return this->history;}
    void User::setName(string name) {
        this->name=name;
    }
    void User::watch(Watchable *watched_content, Session &sess) {
            int index=0;
            bool found=false;
            this->history.push_back(watched_content);
            vector<Watchable*> remaning = get_remaning_watchable();
            if(!remaning.empty()) {
                for (int i = 0; !found && i < remaning.size(); i++) {
                    if (remaning.at(i)->get_id() == watched_content->get_id()) {
                        index = i;
                        found = true;
                    }
                }
                remaning.erase(remaning.begin() + index);
            }
    }
    vector<Watchable *> User::get_remaning_watchable() const {
        return remaning_watchable;
    }
    void User::clear() {
            this->history.clear();
            this->remaning_watchable.clear();
    }
    void User::set_remaning_watchable(vector<Watchable *> new_remaning) {
        for (auto watchable: new_remaning) {
            remaning_watchable.push_back(watchable);
        }
    }
//endregion

//region User - Length Recommender

    //Constructor:
    LengthRecommenderUser::LengthRecommenderUser(const string &name):User(name) ,avgTime(0){}
    LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other):User(other), avgTime(avgTime) {}
    LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser &&other):User(other) , avgTime(other.avgTime)//Move constructor
    {other.clear();}
    LengthRecommenderUser& LengthRecommenderUser::operator=(const LengthRecommenderUser &other)//copy assignment operator
    {
        if(this!=&other){
            this->setName(other.getName());
            this->avgTime=other.avgTime;
            this->clear();

        for(auto watchable:other.history){
            history.push_back(watchable);
        }

        for(auto watchable:other.get_remaning_watchable()){
                vector<Watchable*> new_remaning;
                new_remaning.push_back(watchable);
                this->set_remaning_watchable(new_remaning);
            }

         }
        return *this;
     }
    LengthRecommenderUser& LengthRecommenderUser::operator=(LengthRecommenderUser &&other)//Move assingment operator
    {
        if(this!=&other){
            this->setName(other.getName());
            this->avgTime=other.avgTime;
            this->clear();
            for(auto watchable:other.history) {
                history.push_back(watchable);
            }
            vector<Watchable*> new_remaning;
            for(auto watchable:other.get_remaning_watchable()) {
                new_remaning.push_back(watchable);
            }
            this->set_remaning_watchable(new_remaning);
            other.clear();
        }
        return *this;
    }

    //Methods:
    void LengthRecommenderUser::calculate_avg_time() //calculate the avg time that the user has been watched
    {
        double avg=0;
        for(int i=0;i<history.size();i++){
            Watchable* watch = history.at(i);
            avg=avg+watch->get_length();
        }
        this->avgTime=(avg/history.size());
    }

    Watchable* LengthRecommenderUser::getRecommendation(Session &s)
    {
        Session* session = &s;
        Watchable* nextContent= nullptr;
        vector<Watchable*> remaning = this->get_remaning_watchable();
        this->calculate_avg_time();
        if(!remaning.empty()){
            double timedif=abs(remaning.front()->get_length()- avgTime);
            for(int i=0;i<remaning.size();i++){
                Watchable* content = remaning.at(i);
                if((abs(content->get_length()-this->avgTime))<timedif){
                    timedif = abs(content->get_length()-this->avgTime);
                    nextContent=content;
                }
            }
        }
        return nextContent;
    }
    LengthRecommenderUser *LengthRecommenderUser::clone() {
        return new LengthRecommenderUser(*this);
    }
    string LengthRecommenderUser::toString() {
        string toReturn=this->getName();
        toReturn.append("len");
        return toReturn;
}
//endregion

//region User - Rerun Recommender

    //Constructor
    RerunRecommenderUser::RerunRecommenderUser(const string &name): User(name), index(0){}//parameter constructor
    RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser& other): User(other), index(other.index) //copy constructor
    {}
    RerunRecommenderUser::RerunRecommenderUser(RerunRecommenderUser &&other):User(other), index(other.index)//Move constructor
    {other.clear();}
    RerunRecommenderUser &RerunRecommenderUser::operator=(const RerunRecommenderUser &other)//copy assignment operator
    {
        if(this!=&other){
            this->setName(other.getName());
            this->index=other.get_index();
            this->clear();

            for(auto watchable:other.history){
                history.push_back(watchable);
            }

            for(auto watchable:other.get_remaning_watchable()){
                vector<Watchable*> new_remaning;
                new_remaning.push_back(watchable);
                this->set_remaning_watchable(new_remaning);
            }
        }
        return *this;
    }
    RerunRecommenderUser &RerunRecommenderUser::operator=(RerunRecommenderUser &&other)//move assign operator
    {
        if(this!=&other){
            this->setName(other.getName());
            this->index=other.get_index();
            this->clear();
            for(auto watchable:other.history) {
                history.push_back(watchable);
            }
            vector<Watchable*> new_remaning;
            for(auto watchable:other.get_remaning_watchable()) {
                new_remaning.push_back(watchable);
            }
            this->set_remaning_watchable(new_remaning);
            other.clear();
        }
        return *this;
    }

    //Methods
    Watchable* RerunRecommenderUser::getRecommendation(Session &s)
    {
        Watchable* nextContent= nullptr;
        if(!this->history.empty())
        {
            index = (index+1)%this->history.size();
            nextContent = this->history.at(index);
        }
        return nextContent;
    }
    RerunRecommenderUser *RerunRecommenderUser::clone() {
        return new RerunRecommenderUser(*this);
    }
    string RerunRecommenderUser::toString() {
        string toReturn=this->getName();
        toReturn.append("rer");
        return toReturn;
    }
    int RerunRecommenderUser::get_index() const {
        return this->index;
}
//endregion

//region User - Similar Genre

    // Constructors
    GenreRecommenderUser::GenreRecommenderUser(const string &name): User(name){}
    GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser& other): User(other)
    {
        for(auto elem : other.tags_freq)
            this->tags_freq.insert(pair<string,int>(elem.first,elem.second));
    }
    GenreRecommenderUser::GenreRecommenderUser(GenreRecommenderUser &&other):User(other)//Move constructor
    {
        for(auto elem : other.tags_freq)
            this->tags_freq.insert(pair<string,int>(elem.first,elem.second));
        other.clear();
    }
    GenreRecommenderUser &GenreRecommenderUser::operator=(const GenreRecommenderUser &other) //copy assignment operator
    {
        if(this!=&other){
            this->setName(other.getName());
            this->clear();

            for(auto watchable:other.history){
                history.push_back(watchable);
            }

            for(auto watchable:other.get_remaning_watchable()){
                vector<Watchable*> new_remaning;
                new_remaning.push_back(watchable);
                this->set_remaning_watchable(new_remaning);
            }
            for(auto elem : other.tags_freq)
                this->tags_freq.insert(pair<string,int>(elem.first,elem.second));
        }
        return *this;
    }
    GenreRecommenderUser &GenreRecommenderUser::operator=(GenreRecommenderUser &&other) //move assign operator
    {
        if(this!=&other){
            this->setName(other.getName());
            this->clear();
            for(auto watchable:other.history) {
                history.push_back(watchable);
            }
            vector<Watchable*> new_remaning;
            for(auto watchable:other.get_remaning_watchable()) {
                new_remaning.push_back(watchable);
            }
            this->set_remaning_watchable(new_remaning);
            for(auto elem : other.tags_freq)
                this->tags_freq.insert(pair<string,int>(elem.first,elem.second));

            other.clear();
        }
        return *this;
    }
    GenreRecommenderUser::~GenreRecommenderUser() {
        this->clear();
    }

//Methods:

    void GenreRecommenderUser::add_tag_freq(vector<string> tags)
    {
        for(auto tag:tags){
            this->tags_freq[tag]++;

        }
    }
    Watchable* GenreRecommenderUser::getRecommendation(Session &s)
    {
        int freq=0;
        string most_popular_tag;
        Watchable* nextContent=nullptr;

        vector<pair<string, int>> sort_tags  = this->sort_tags();//tags sorts in descending order by the second value first(freq) and if equal so by lexicographic order
        for (pair<string, int> tag : sort_tags){
            std::cout << tag.first << " :: " << tag.second << std::endl;//for debugging
            if(nextContent==nullptr){
                nextContent = get_content_by_tag(tag.first);
            }
        }
        return nextContent;
    }
    GenreRecommenderUser *GenreRecommenderUser::clone() {
        return new GenreRecommenderUser(*this);
    }
    string GenreRecommenderUser::toString() {
        string toReturn=this->getName();
        toReturn.append("gen");
        return toReturn;
    }
    map<string, int> GenreRecommenderUser::get_tags_freq() {
        return this->tags_freq;
    }
    vector<pair<string,int>> GenreRecommenderUser::sort_tags() {

        // Declaring the type of Predicate that accepts 2 pairs and return a boolean
        typedef function<bool(pair<string, int>, pair<string, int>)> Comparator;

        // Defining a lambda function to compare two pairs. It will compare two pairs using second field
        Comparator compFunctor =
                [](pair<string, int> elem1 ,pair<string, int> elem2)
                {bool toreturn;
                    if(elem1.second>elem2.second)
                    {
                        toreturn=true;
                    }
                    else if(elem1.second<elem2.second)
                    {
                        toreturn=false;
                    }
                    else if(elem1.second==elem2.second)
                    {
                        if(elem1.first>elem2.first){
                            toreturn=true;
                        }
                        else if(elem1.first<elem2.first){
                            toreturn=false;
                        }
                    }
                    return toreturn;
                };

    // Declaring a set that will store the pairs using above comparision logic
    set<pair<string, int>, Comparator> sortedSet(
            tags_freq.begin(), tags_freq.end(), compFunctor);
    vector<pair<string,int>>  toReturn;
    for(auto pair : sortedSet){
        toReturn.push_back(pair);
    }

    return toReturn;
}
    Watchable *GenreRecommenderUser::get_content_by_tag(string tag) {
        for(auto watchable :this->get_remaning_watchable()){
            for(auto watchableTag: watchable->get_tags()){
                if(watchableTag==tag){
                    return watchable;
                }
            }
        }
        return nullptr;
    }
    void GenreRecommenderUser::clear() {
        User::clear();
        this->tags_freq.clear();
    }
    void GenreRecommenderUser::watch(Watchable *watched_content, Session &sess) {
        this->add_tag_freq(watched_content->get_tags());
        User::watch(watched_content, sess);
    }


//endregion