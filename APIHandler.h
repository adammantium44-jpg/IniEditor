#pragma once
#include "PrismaUI/PrismaUI_API.h"
#include "PMCM.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <unordered_map>
#include <fmt/std.h>   


namespace Req_PrismaUI_API
{
	inline PRISMA_UI_API::IVPrismaUI1* PrismaUI = nullptr;
	inline PrismaView PView = 0;
	inline PrismaView ConfigView = 0;
	inline PrismaView LastConfigView = 0;
	static inline Settings gCfg;

	static const std::filesystem::path kViewRoot = std::filesystem::path("Data/PrismaUI/views").lexically_normal();
	
	static std::unordered_map<std::string, std::string> gFileCache;

	void Request_PrismaUI_API();
	void InitializeMCM();
	void FocusReload();

	std::string LoadPMCMJsons_CommaJoined();
	void passConfig();
	inline std::string menuJson;

	void ResetMenu();
};

