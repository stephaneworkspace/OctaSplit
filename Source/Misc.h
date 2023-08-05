//
// Created by Stéphane on 05.08.23.
//
#pragma once
#include <string>
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

class Misc {
public:
    Misc();
    string getAppName();
    string getAppVersion();
private:
    static string getExecutablePath();

    string appName;
    string appVersion;
};