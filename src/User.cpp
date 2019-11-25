#include <include/Watchable.h>
#include "include/User.h"
#include <cmath>        // std::abs
#include <algorithm>    // std::max
using namespace std;

//region User abstract

    //Constructors
    User::User(const string &name): name(name) {}//parameter constructor
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
        }
        return *this;
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
    string User::getName() const {return this->name;}
    vector<Watchable*> User::get_history() const {return this->history;}

    void User::setName(string name) {
        this->name=name;
    }

void User::watch(Watchable *watched_content) {
        this->history.push_back(watched_content);
}


//endregion

//region User - Length Recommender

    //Constructor:
    LengthRecommenderUser::LengthRecommenderUser(const string &name):User(name){}
    LengthRecommenderUser::LengthRecommenderUser(const LengthRecommenderUser &other):User(other), avgTime(avgTime) {}
    LengthRecommenderUser::LengthRecommenderUser(LengthRecommenderUser &&other):User(other), avgTime(other.avgTime)//Move constructor
    {
    //TODO: do we need to clear the pointer of the vector?
    }
    LengthRecommenderUser& LengthRecommenderUser::operator=(const LengthRecommenderUser &other){}//copy assignment operator
    LengthRecommenderUser& LengthRecommenderUser::operator=(LengthRecommenderUser &&other){}//Move assingment operator

    //Methods:
    ostream& operator << (ostream& os, const LengthRecommenderUser &user) {
        return (os << user.getName() << "\n Recommendation algo: len "<< endl);
    }

    double LengthRecommenderUser::calculate_avg_time() //calculate the avg time that the user has been watched
    {
        double avg=0;
        for(int i=0;i<history.size();i++){
            avg=avg+history.at(i)->get_length();
        }
        this->avgTime=(avg/history.size());
    }
    void LengthRecommenderUser::set_remaning_watchable(vector<Watchable *> all_content)
    {
        vector<Watchable*> remaning_content ;
        for(int i=0;i<all_content.size();i++) {
            if (find(this->history.begin(), this->history.end(), all_content.at(i)) != this->history.end()) {
                /* history contains all_content.at(i) */
            } else {
                /* history does not contain all_content.at(i) */
                remaning_content.push_back(all_content.at(i))
            }
        }
        this->remaning_watchable=remaning_content;
    }
    Watchable* LengthRecommenderUser::getRecommendation(Session &s)
    {
        Session* session = &s;
        Watchable* nextContent= nullptr;
        set_remaning_watchable(session.get_content())

        if(!this->remaning_watchable.empty()){
            double timedif=abs(this->remaning_watchable.front()->get_length()- avgTime);
            for(int i=0;i<remaning_content.size();i++){
                if((abs(remaning_content.at(i)-this->avgTime))<timedif){
                    timedif = abs(remaning_content.at(i)-this->avgTime);
                    nextContent=remaning_content.at(i);
                }
            }
        }
        return nextContent;
    }

    LengthRecommenderUser *LengthRecommenderUser::clone() {
        return new LengthRecommenderUser(&this);
    }

    void LengthRecommenderUser::watch(Watchable *watched_content) {
        this->history.push_back(watched_content);

    }
//endregion

//region User - Rerun Recommender

    //Constructor
    RerunRecommenderUser::RerunRecommenderUser(const string &name): User(name), index(0){}
    RerunRecommenderUser::RerunRecommenderUser(const RerunRecommenderUser& other): User(other), index(other.index) {}

    //Methods
    ostream& operator << (ostream& os, const RerunRecommenderUser &user) {
        return (os << user.getName() << "\n Recommendation algo: rer "<< endl);
    }
    Watchable* RerunRecommenderUser::getRecommendation(Session &s)
    {
        Watchable* nextContent= nullptr;
        if(!this->history.empty())
        {
            nextContent = this->history.at(index);
            index = (index+1)%this->history.size();
        }
        return nextContent;
    }

RerunRecommenderUser *RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(&this);
}
//endregion

//region User - Similar Genre

    // Constructors
    GenreRecommenderUser::GenreRecommenderUser(const string &name): User(name){}
    GenreRecommenderUser::GenreRecommenderUser(const GenreRecommenderUser& other): User(other)
    {
        for(auto elem : other.tags_freq)
            this->tags_freq.insert(elem.first,elem.second);

        for(auto elem: other.remaning_watchable)
            this->remaning_watchable.push_back(elem->clone())
    }

    //Methods:
    ostream& operator << (ostream& os, const RerunRecommenderUser &user) {
        return (os << user.getName() << "\n Recommendation algo: gen "<< endl);
    }
    void GenreRecommenderUser::add_tag_freq(const string &tag)
    {
        this->tags_freq[tag]++
    }
    void GenreRecommenderUser::set_remaning_watchable(vector<Watchable *> all_content)
    {
        vector<Watchable*> remaning_content ;
        for(int i=0;i<all_content.size();i++) {
            if (find(this->history.begin(), this->history.end(), all_content.at(i)) != this->history.end()) {
                /* history contains all_content.at(i) */
            } else {
                /* history does not contain all_content.at(i) */
                remaning_content.push_back(all_content.at(i))
            }
        }
        this->remaning_watchable=remaning_content;
    }
    Watchable* GenreRecommenderUser::getRecommendation(Session &s)
    {
        int freq=0;
        string most_popular_tag;
        Watchable* nextContent=nullptr;

        //finding the most popular tags among the hash map elements
        unordered_map<string, int>:: iterator iter;
        for (iter = this->tags_freq.begin(); iter != this->tags_freq.end(); iter++) {
            cout << "(" << iter->first << ", " << iter->second << ")\n";
            if(iter->second >=freq){
                if(iter->second==freq){
                    most_popular_tag=max(most_popular_tag,iter->first);
                }
                if(iter->second>freq){
                    freq=iter->second;
                    most_popular_tag=iter->first;
                }
            }
        }
        //find the first watchable at remaining content which contains the most popular tag
        for(int i=0;nextContent== nullptr && i<remaning_watchable.size();i++){

            vector<srting> watchable_tags = remaning_watchable.at(i).get_tags();
            if(find(watchable_tags.begin(), watchable_tags.end(), most_popular_tag) != watchable_tags.end()) {
                /* watchable_tags contains most_popular_tag */
                nextContent=remaning_watchable.at(i)
            } else {
                /* watchable_tags does not contain most_popular_tag */
            }
        }
        return nextContent;
    }

GenreRecommenderUser *GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(&this);

}
//endregion