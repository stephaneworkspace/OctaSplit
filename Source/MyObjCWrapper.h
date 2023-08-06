//
// Created by Stéphane on 06.08.23.
//

#pragma once

#ifdef JUCE_MAC
void ShowMacOSDialog();
#else
inline void ShowMacOSDialog() {}
#endif