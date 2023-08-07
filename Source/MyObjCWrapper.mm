#import <Cocoa/Cocoa.h>
#include "MyObjCWrapper.h"

void ShowMacOSDialog()
{
    NSString *appPath = [[NSBundle mainBundle] bundlePath];
    NSString *path = [appPath stringByAppendingPathComponent:@"Contents/MacOS/"];

    // Création d'une alerte pour afficher le chemin
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:@"Hello World"];
    [alert setInformativeText:path]; // Affichage du chemin du fichier .env
    [alert addButtonWithTitle:@"OK"];
    [alert runModal];
}

NSString* GetPathObjC() {
    NSString *appPath = [[NSBundle mainBundle] bundlePath];
    NSString *path = [appPath stringByAppendingPathComponent:@"Contents/MacOS/"];
    return path;
}

std::string GetPath() {
    NSString* envPath = GetPathObjC();
    return std::string([envPath UTF8String]);
}