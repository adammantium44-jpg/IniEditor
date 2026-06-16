#include "PCH.h"
#include "IniEditor.h"
#include "SKSEMenuFramework.h"

SKSEPluginLoad(const SKSE::LoadInterface*)
{
    WritePMCMConfig();

    return true;
}
