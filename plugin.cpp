#include "PCH.h"
#include "Plugin.h"
#include "IniEditorUI.h"

using namespace SKSE;

void OnMessage(SKSE::MessagingInterface::Message* msg)
{
    if (msg->type == SKSE::MessagingInterface::kPostLoad) {
        IniEditorUI::Register();
    }
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const LoadInterface* skse)
{
    Init(skse);

    auto messaging = GetMessagingInterface();
    messaging->RegisterListener("SKSE", OnMessage);

    return true;
}