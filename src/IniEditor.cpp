#include "../PCH.h"
#include "IniEditor.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <cctype>

void IniEditor::RegisterMenu()
{
    // Pastikan framework terinstall
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }

    // Set root key menu kita, misal "IniEditor"
    SKSEMenuFramework::Model::Internal::key = "IniEditor";

    // Scan semua file .ini di Data/SKSE/Plugins
    ScanIniFiles();

    // Kalau ada minimal satu file, pilih yang pertama sebagai default
    if (!s_iniFiles.empty()) {
        s_selectedIni = s_iniFiles.front();
        LoadIni();
    }

    // Daftarkan satu section item, misal "General"
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

void IniEditor::DrawPage()
{
    // Untuk sementara, kosongkan. SKSE Menu Framework akan tetap memanggil fungsi ini,
    // tapi kita belum menggambar apa-apa sampai ImGui-nya di-handle dengan benar.

    // Nanti di sini:
    // - Tampilkan list s_iniFiles (dropdown/list)
    // - Kalau user pilih file lain:
    //      s_selectedIni = file;
    //      LoadIni();
    // - Tampilkan key2 dari s_bools/s_floats/s_strings sebagai kontrol
    // - Panggil SaveIni() ketika user mengubah atau klik "Save"
}

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
        // komentar dengan ; atau #
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

        // Deteksi bool
        if (value == "true" || value == "false") {
            s_bools[key] = (value == "true");
            continue;
        }

        // Deteksi angka (float)
        char* end = nullptr;
        float f = std::strtof(value.c_str(), &end);
        if (end != value.c_str() && *end == '\0') {
            s_floats[key] = f;
            continue;
        }

        // Sisanya string
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

    // Untuk sekarang, kita tulis tanpa grouping section
    // Nanti bisa dikembangkan dengan [Section] kalau perlu.

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
