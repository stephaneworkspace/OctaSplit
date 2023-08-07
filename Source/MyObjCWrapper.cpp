//
// Created by Stéphane on 07.08.23.
//
#include <string>
#include "MyObjCWrapper.h"

#ifdef JUCE_MAC
// Déclarez la fonction Objective-C pour qu'elle puisse être utilisée en C++
extern "C" NSString* GetEnvPathObjC();

std::string GetEnvPath() {
    NSString* envPath = GetEnvPathObjC();
    return std::string([envPath UTF8String]);
}
#else
inline std::string GetEnvPath() {
    return "";
}
#endif
