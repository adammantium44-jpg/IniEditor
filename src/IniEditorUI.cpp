#include "PCH.h"
#include "IniEditorUI.h"

#include <fstream>
#include <sstream>
#include <cctype>
#include <imgui.h>

namespace {
    bool IsSupportedFile(const std::filesystem::path& path)
    {
        return path.extension() == ".ini";
    }

    void Log(const std::string& msg)
    {
        logger::info("[IniEditor] {}", msg);
    }
}

namespace IniEditorUI {

    void Register()
    {
        if (!SKSEMenuFramework::IsInstalled()) {
            Log("SKSE Menu Framework not installed, skipping UI registration");
            return;
        }

        // key wajib unik per plugin
        SKSEMenuFramework::Model::Internal::key = "IniEditor";

        // window utama editor
        EditorWindow = SKSEMenuFramework::AddWindow(RenderEditorWindow, true);
        if (!EditorWindow) {
            Log("Failed to create EditorWindow");
        }

        // section di menu kiri
        SKSEMenuFramework::AddSectionItem("IniEditor", RenderExplorer);

        // Untuk keamanan: jangan langsung load di sini
        // Biar user klik Refresh dulu supaya kelihatan kalau ada error
        // RefreshFileList();
        // LoadCurrentFile();
    }

    void RefreshFileList()
    {
        FileList.clear();
        CurrentOpenFile.clear();

        const std::filesystem::path root{ RootFolder };
        if (!std::filesystem::exists(root)) {
            Log("Root folder does not exist: " + root.string());
            return;
        }

        // Lebih aman: non-recursive dulu, baru nanti kalau mau bisa diganti recursive
        for (const auto& entry : std::filesystem::directory_iterator(root)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            const auto& path = entry.path();
            if (IsSupportedFile(path)) {
                FileList.push_back(path);
            }
        }

        if (!FileList.empty()) {
            CurrentOpenFile = FileList.front();
            Log("Initial file: " + CurrentOpenFile.string());
        } else {
            Log("No ini files found in root folder");
        }
    }

    void LoadCurrentFile()
    {
        Bools.clear();
        Floats.clear();
        Strings.clear();

        if (CurrentOpenFile.empty()) {
            Log("LoadCurrentFile called with empty CurrentOpenFile");
            return;
        }

        if (!std::filesystem::is_regular_file(CurrentOpenFile)) {
            Log("CurrentOpenFile is not a regular file: " + CurrentOpenFile.string());
            return;
        }

        std::ifstream file(CurrentOpenFile);
        if (!file.is_open()) {
            Log("Failed to open file: " + CurrentOpenFile.string());
            return;
        }

        auto trim = [](std::string& s) {
            while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
            while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
        };

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
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
                Bools[key] = (value == "true");
                continue;
            }

            char* end = nullptr;
            float f = std::strtof(value.c_str(), &end);
            if (end != value.c_str() && *end == '\0') {
                Floats[key] = f;
                continue;
            }

            Strings[key] = value;
        }

        Log("Loaded file: " + CurrentOpenFile.string());
    }

    void SaveCurrentFile()
    {
        if (CurrentOpenFile.empty()) {
            Log("SaveCurrentFile called with empty CurrentOpenFile");
            return;
        }

        std::ofstream file(CurrentOpenFile);
        if (!file.is_open()) {
            Log("Failed to save file: " + CurrentOpenFile.string());
            return;
        }

        for (const auto& [key, value] : Bools) {
            file << key << " = " << (value ? "true" : "false") << '\n';
        }

        for (const auto& [key, value] : Floats) {
            file << key << " = " << value << '\n';
        }

        for (const auto& [key, value] : Strings) {
            file << key << " = " << value << '\n';
        }

        Log("Saved file: " + CurrentOpenFile.string());
    }

    // === RENDER ===

    void __stdcall RenderExplorer()
    {
        ImGui::Text("IniEditor");
        ImGui::Separator();

        if (ImGui::Button("Refresh list")) {
            RefreshFileList();
        }

        if (EditorWindow && ImGui::Button("Open editor window")) {
            EditorWindow->IsOpen = true;
        }

        if (FileList.empty()) {
            ImGui::Text("No ini files found in:");
            ImGui::Text("%s", RootFolder.c_str());
            return;
        }

        ImGui::Text("Files in %s", RootFolder.c_str());
        ImGui::Separator();

        for (auto& path : FileList) {
            bool selected = (path == CurrentOpenFile);
            if (ImGui::Selectable(path.filename().string().c_str(), selected)) {
                CurrentOpenFile = path;
                LoadCurrentFile();
            }
        }
    }

    void __stdcall RenderEditorWindow()
    {
        if (!EditorWindow || !EditorWindow->IsOpen) {
            return;
        }

        if (CurrentOpenFile.empty()) {
            ImGui::Text("No file selected.");
            return;
        }

        ImGui::Text("Editing: %s", CurrentOpenFile.filename().string().c_str());
        ImGui::Separator();

        if (ImGui::Button("Save")) {
            SaveCurrentFile();
        }

        ImGui::Separator();
        ImGui::Text("Bools:");
        for (auto& [key, value] : Bools) {
            bool v = value;
            if (ImGui::Checkbox(key.c_str(), &v)) {
                value = v;
            }
        }

        ImGui::Separator();
        ImGui::Text("Floats:");
        for (auto& [key, value] : Floats) {
            float v = value;
            if (ImGui::SliderFloat(key.c_str(), &v, -1000.0f, 1000.0f)) {
                value = v;
            }
        }

        ImGui::Separator();
        ImGui::Text("Strings:");
        for (auto& [key, value] : Strings) {
            std::string buf = value;
            char buffer[256];
            std::snprintf(buffer, sizeof(buffer), "%s", buf.c_str());
            if (ImGui::InputText(key.c_str(), buffer, sizeof(buffer))) {
                value = buffer;
            }
        }
    }
}