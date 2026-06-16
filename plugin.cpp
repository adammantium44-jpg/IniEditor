#include "PCH.h"
#include "PrismaConfig.h"
#include "SKSEMenuFramework.h"

SKSEPluginLoad(const SKSE::LoadInterface*)
{
    WritePMCMConfig();

    return true;
}
