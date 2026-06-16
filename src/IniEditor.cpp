#include "../PCH.h"
#include "IniEditor.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <cctype>
#include <imgui.h>

// ---------------- Register & scanning ----------------

void IniEditor::RegisterMenu()
{
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }

    SKSEMenuFramework::Model::Internal::key = "IniEditor";

    ScanIniFiles();

    if (!s_iniFiles.empty()) {
        s_selectedIni = s_iniFiles.front();
        LoadIni();
    }

    // Daftarkan RenderPage (signature __stdcall, free/static function)
    SKSEMenuFramework::AddSectionItem("General", &IniEditor::RenderPage);
}

void __stdcall IniEditor::RenderPage()
{
    // Wrapper supaya signature cocok dengan RenderFunction
    DrawPage();
}

void IniEditor::ScanIniFiles()
{
    s_iniFiles.clear();

    const std::filesystem::path root{"Data/SKSE/Plugins"};
    if (!std::filesystem::exists(root)) {
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(root)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        const auto& path = entry.path();
        if (path.extension() == ".ini") {
            s_iniFiles.push_back(path.string());
        }
    }
}

// ---------------- UI ----------------

void IniEditor::DrawPage()
{
    ImGui::Text("Test");
}

// ---------------- INI load/save ----------------

void IniEditor::LoadIni()
{
    s_bools.clear();
    s_floats.clear();
    s_strings.clear();

    if (s_selectedIni.empty()) {
        return;
    }

    // Kalau path bukan file biasa, jangan apa-apa
    if (!std::filesystem::is_regular_file(s_selectedIni)) {
        return;
    }

    std::ifstream file(s_selectedIni);
    if (!file.is_open()) {
        return;
    }

    std::string line;

    auto trim = [](std::string& s) {
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
        while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
    };

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        // komentar
        if (line[0] == ';' || line[0] == '#') {
            continue;
        }

        auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        trim(key);
        trim(value);

        if (key.empty()) {
            continue;
        }

        if (value == "true" || value == "false") {
            s_bools[key] = (value == "true");
            continue;
        }

        char* end = nullptr;
        float f = std::strtof(value.c_str(), &end);
        if (end != value.c_str() && *end == '\0') {
            s_floats[key] = f;
            continue;
        }

        s_strings[key] = value;
    }
}

void IniEditor::SaveIni()
{
    if (s_selectedIni.empty()) {
        return;
    }

    std::ofstream file(s_selectedIni);
    if (!file.is_open()) {
        return;
    }

    // Bools
    for (const auto& [key, value] : s_bools) {
        file << key << " = " << (value ? "true" : "false") << '\n';
    }

    // Floats
    for (const auto& [key, value] : s_floats) {
        file << key << " = " << value << '\n';
    }

    // Strings
    for (const auto& [key, value] : s_strings) {
        file << key << " = " << value << '\n';
    }
}
