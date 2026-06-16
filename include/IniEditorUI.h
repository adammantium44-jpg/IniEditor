#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <map>

#include "SKSEMenuFramework.h"

namespace IniEditorUI {
    void Register();  // dipanggil dari SKSEPlugin_Load

    inline std::string RootFolder = "Data\\SKSE\\Plugins"; // atau folder khususmu

    inline std::vector<std::filesystem::path> FileList;
    inline std::filesystem::path CurrentOpenFile;

    inline std::map<std::string, bool> Bools;
    inline std::map<std::string, float> Floats;
    inline std::map<std::string, std::string> Strings;

    inline MENU_WINDOW EditorWindow;

    void RefreshFileList();
    void LoadCurrentFile();
    void SaveCurrentFile();

    void __stdcall RenderExplorer();
    void __stdcall RenderEditorWindow();
}