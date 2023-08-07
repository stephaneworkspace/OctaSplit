//
// Created by Stéphane on 07.08.23.
//
#include "MyObjCWrapper.h"

#ifdef JUCE_MAC
// Déclarez la fonction Objective-C pour qu'elle puisse être utilisée en C++
extern "C" NSString* GetPathObjC();

std::string GetPath() {
    NSString* envPath = GetPathObjC();
    return std::string([envPath UTF8String]);
}
#endif
