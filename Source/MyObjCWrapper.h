//
// Created by Stéphane on 06.08.23.
//

#pragma once
#include <string>

#ifdef __APPLE__
// Cette fonction retourne le chemin du fichier .env sur macOS, ou une chaîne vide sur d'autres plateformes.
std::string GetPath();
// Cette fonction affiche une boîte de dialogue avec le message "Hello World" sur macOS.
// Sur d'autres plateformes, elle ne fait rien.
void ShowMacOSDialog();
#else
inline void ShowMacOSDialog() {}
inline std::string GetPath() {
    return "";
}
#endif