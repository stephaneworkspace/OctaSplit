//
// Created by Stéphane on 05.08.23.
//
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "MyObjCWrapper.h"

using namespace std;

class Misc {
public:
    Misc();
    string getAppName();
    string getAppVersion();
private:

    string appName;
    string appVersion;
};