//
// Created by Stéphane on 05.08.23.
//

#include "Misc.h"

Misc::Misc() {
    ifstream file(".env");

    // Si l'ouverture du fichier avec un chemin relatif échoue
    if (!file.is_open()) {
#ifdef __linux__
        file.open("/opt/OctaSplit/.env");
#endif
#ifdef __APPLE__
// Code spécifique à macOS
        file.open(GetPath() + "/.env");
#endif
    }

    if (!file.is_open()) {
        std::cout << "Failed to open .env file.\n" << endl;
    }

    string line;
    map<string, string> env;

    while (getline(file, line)) {
        istringstream iss(line);
        string key, val;
        getline(getline(iss, key, '='), val);
        env[key] = val;
    }

    appName = env.count("APP_NAME") ? env["APP_NAME"] : "OctaSplit";
    appVersion = env.count("APP_VERSION") ? env["APP_VERSION"] : "Unknown Version";
}

string Misc::getAppName() {
    return appName;
}

string Misc::getAppVersion() {
    return appVersion;
}