#include "PCH.h"
#include "IniEditor.h"

void IniEditor::RegisterMenu()
{
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }

    // Contoh API, nanti kita sesuaikan dengan versi framework kamu
    // Misal untuk SKSE Menu Framework 2:
    // SKSEMenuFramework::RegisterMenu("Ini Editor", &IniEditor::DrawPage);

    // TODO: cek Usage.md dari SDK dan sesuaikan pemanggilan di sini
}

void IniEditor::DrawPage()
{
    // Untuk test awal, cukup gambar text statis
    if (!ImGui::Begin("Ini Editor")) {
        ImGui::End();
        return;
    }

    ImGui::Text("Ini Editor is working.");
    ImGui::End();
}

void IniEditor::LoadIni()
{
    // Stub, nanti diisi
}

void IniEditor::SaveIni()
{
    // Stub, nanti diisi
}
