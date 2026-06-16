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

    SKSEMenuFramework::AddSectionItem("General", &IniEditor::DrawPage);
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
    // Dropdown pilih file INI
    const char* currentLabel = s_selectedIni.empty()
        ? "<tidak ada>"
        : std::filesystem::path(s_selectedIni).filename().string().c_str();

    if (ImGui::BeginCombo("Pilih INI", currentLabel)) {
        for (const auto& fullPath : s_iniFiles) {
            bool isSelected = (fullPath == s_selectedIni);
            std::string filename = std::filesystem::path(fullPath).filename().string();

            if (ImGui::Selectable(filename.c_str(), isSelected)) {
                s_selectedIni = fullPath;
                LoadIni();
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (s_selectedIni.empty()) {
        ImGui::Text("Tidak ada file INI yang dipilih.");
        return;
    }

    ImGui::Text("File: %s", s_selectedIni.c_str());
    ImGui::Separator();

    // Booleans
    if (!s_bools.empty()) {
        ImGui::Text("Booleans");
        for (auto& [key, value] : s_bools) {
            ImGui::Checkbox(key.c_str(), &value);
        }
        ImGui::Separator();
    }

    // Floats
    if (!s_floats.empty()) {
        ImGui::Text("Floats");
        for (auto& [key, value] : s_floats) {
            ImGui::SliderFloat(key.c_str(), &value, -1000.0f, 1000.0f);
        }
        ImGui::Separator();
    }

    // Strings
    if (!s_strings.empty()) {
        ImGui::Text("Strings");
        for (auto& [key, value] : s_strings) {
            char buffer[256];
            std::snprintf(buffer, sizeof(buffer), "%s", value.c_str());
            if (ImGui::InputText(key.c_str(), buffer, sizeof(buffer))) {
                value = buffer;
            }
        }
        ImGui::Separator();
    }

    if (ImGui::Button("Save")) {
        SaveIni();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        LoadIni();
    }
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
