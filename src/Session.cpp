#include "include/Session.h"
#include <../include/json.hpp>
#include <fstream>
#include <iostream>

//Constructors
    Session::Session(const std::string &configFilePath) {
    using json= nlohamann::json;
    std::ifstream i(configFilePath);
    json j;
    i >> j;
    std:: cout << j << std::endl;
}
    Session::Session(const Session &other) {}

//Methods:
    void Session::start() {}