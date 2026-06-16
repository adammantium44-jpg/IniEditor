#include "PCH.h"
#include "IniEditor.h"
#include "SKSEMenuFramework.h"

// Macro SKSEPluginLoad biasanya diexpand jadi:
// extern "C" __declspec(dllexport) bool SKSEPlugin_Load(const SKSE::LoadInterface* skse)
SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    // Inisialisasi CommonLibSSE-NG
    SKSE::Init(skse);

    // Dulu di sini ada WritePMCMConfig(); (Prisma/PMCM) -> tidak dipakai lagi, jadi dihapus

    // Registrasi menu ke SKSE Menu Framework
    IniEditor::RegisterMenu();

    return true;
}
