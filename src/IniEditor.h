#pragma once

#include "SKSEMenuFramework.h"
#include <string>
#include <map>
#include <vector>

class IniEditor
{
public:
    static void RegisterMenu();
    static void DrawPage(); // implementasi UI

private:
    static void ScanIniFiles();
    static void LoadIni();
    static void SaveIni();

    static inline std::vector<std::string> s_iniFiles{};
    static inline std::string s_selectedIni{};

    static inline std::map<std::string, bool> s_bools{};
    static inline std::map<std::string, float> s_floats{};
    static inline std::map<std::string, std::string> s_strings{};
};

// Fungsi global yang di-register ke SKSE Menu Framework
void __stdcall IniEditor_RenderPage();