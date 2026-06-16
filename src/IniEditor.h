#pragma once

#include "SKSEMenuFramework.h"
#include <string>
#include <map>
#include <vector>

class IniEditor
{
public:
    static void RegisterMenu();

private:
    static void DrawPage();

    static void ScanIniFiles();
    static void LoadIni();
    static void SaveIni();

    // Daftar semua INI di Data/SKSE/Plugins
    static inline std::vector<std::string> s_iniFiles{};

    // File INI yang sedang dipilih/aktif
    static inline std::string s_selectedIni{};

    // Data key/value dari file aktif
    static inline std::map<std::string, bool> s_bools{};
    static inline std::map<std::string, float> s_floats{};
    static inline std::map<std::string, std::string> s_strings{};
};
