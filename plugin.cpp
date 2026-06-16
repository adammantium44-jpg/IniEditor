#include "PCH.h"
#include "PrismaConfig.h"

SKSEPluginLoad(const SKSE::LoadInterface*)
{
    WritePMCMConfig();

    return true;
}
