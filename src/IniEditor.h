#pragma once

#include "SKSEMenuFramework.h"
#include <string>
#include <map>

class IniEditor
{
public:
    static void RegisterMenu();

private:
    static void DrawPage();

    static void LoadIni();
    static void SaveIni();

    static inline std::string s_iniPath{};
    static inline std::map<std::string, bool> s_bools{};
    static inline std::map<std::string, float> s_floats{};
    static inline std::map<std::string, std::string> s_strings{};
};
