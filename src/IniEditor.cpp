#include "PCH.h"
#include "IniEditor.h"

// ImGui (exposed via SKSE Menu Framework)
#include <imgui.h>

void IniEditor::RegisterMenu()
{
    // Pastikan framework terinstall
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }

    // Set root key menu kita, misal "IniEditor"
    SKSEMenuFramework::Model::Internal::key = "IniEditor";

    // Daftarkan satu section item, misal "General"
    SKSEMenuFramework::AddSectionItem("General", &IniEditor::DrawPage);

    // Nanti kalau mau banyak page:
    // SKSEMenuFramework::AddSectionItem("Gameplay", &IniEditor::DrawGameplayPage);
    // SKSEMenuFramework::AddSectionItem("UI", &IniEditor::DrawUIPage);
}

void IniEditor::DrawPage()
{
    // Untuk sementara, kosongkan. SKSE Menu Framework akan tetap memanggil fungsi ini,
    // tapi kita belum menggambar apa-apa sampai ImGui-nya di-handle dengan benar.
}

void IniEditor::LoadIni()
{
    // TODO: implementasikan parsing .ini
    // s_iniPath = "Data/SKSE/Plugins/YourMod.ini";
    // Baca file, isi s_bools/s_floats/s_strings
}

void IniEditor::SaveIni()
{
    // TODO: implementasikan penulisan .ini
    // Simpan s_bools/s_floats/s_strings ke s_iniPath
}
