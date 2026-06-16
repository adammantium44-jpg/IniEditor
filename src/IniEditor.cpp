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
    // Untuk sekarang, simple test UI
    // Di SKSE Menu Framework, ImGui context sudah di-setup

    ImGui::Text("Ini Editor - test page");

    // Contoh dummy control
    static bool exampleBool = false;
    static float exampleFloat = 1.0f;

    if (ImGui::Checkbox("Example toggle", &exampleBool)) {
        // Nanti bisa update s_bools["ExampleToggle"] dan SaveIni()
    }

    if (ImGui::SliderFloat("Example value", &exampleFloat, 0.0f, 10.0f)) {
        // Nanti bisa update s_floats["ExampleValue"] dan SaveIni()
    }

    if (ImGui::Button("Save INI")) {
        SaveIni();
    }
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
