//
// Created by Stéphane on 06.08.23.
//

#ifndef PATH_WRAPPER_H
#define PATH_WRAPPER_H

#include <string>

// Cette fonction retourne le chemin du fichier .env sur macOS, ou une chaîne vide sur d'autres plateformes.
std::string GetPath();

// Cette fonction affiche une boîte de dialogue avec le message "Hello World" sur macOS.
// Sur d'autres plateformes, elle ne fait rien.
void ShowMacOSDialog();

#if !defined(JUCE_MAC)
inline void ShowMacOSDialog() {}
#endif

#endif // PATH_WRAPPER_H
