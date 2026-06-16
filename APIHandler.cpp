#include "APIHandler.h"
#include <string_view>

namespace fs = std::filesystem;

namespace Req_PrismaUI_API
{

	void Request_PrismaUI_API()
	{
		if (!PrismaUI) {
			PrismaUI = static_cast<PRISMA_UI_API::IVPrismaUI1*>(PRISMA_UI_API::RequestPluginAPI(PRISMA_UI_API::InterfaceVersion::V1));
			InitializeMCM();
		}
	}

	void InitializeMCM() {
		if (!PrismaUI)
			return;

		PView = PrismaUI->CreateView("PMCM/shell.html", [](PrismaView view) -> void {
			logger::info("PMCM PrismaUI is load : {}", view);

			PrismaUI->SetOrder(PView, 10000);
			PrismaUI->SetScrollingPixelSize(PView, 180);
		});

		PrismaUI->Hide(PView);

		 PrismaUI->RegisterJSListener(PView, "focustg", [](const char* arg) {
			std::string action = arg;

			 if (action == "close") {
				if (!PrismaUI->IsHidden(ConfigView) || !PrismaUI->IsHidden(PView)) {
					PrismaUI->Hide(ConfigView);
					PrismaUI->Hide(PView);
				} 
			} 
		});


		PrismaUI->RegisterJSListener(PView, "LoadModConfigView", [](const char* argJson) {
			try {
				auto j = json::parse(argJson ? argJson : "{}");
				std::string rel = j.value("viewID", "");
				if (rel.empty()) {
					return;
				}

				ConfigView = std::stoull(rel);	
				if (LastConfigView != 0 && LastConfigView != ConfigView) {
					PrismaUI->Hide(LastConfigView);
				}

				LastConfigView = ConfigView;
				PrismaUI->SetOrder(ConfigView, 10001);
				PrismaUI->Show(ConfigView);
				//PrismaUI->Focus(ConfigView);

				PrismaUI->RegisterJSListener(ConfigView, "autoFocusView", [](const char* arg) {
					std::string region = arg;

					if (region == "left") {
						PrismaUI->Focus(PView);
					} else if (region == "right") {
						if (!PrismaUI->IsHidden(ConfigView)) {
							PrismaUI->Focus(ConfigView);
						}
					} 
				});

				PrismaUI->RegisterJSListener(PView, "autoFocusView", [](const char* arg) {
					std::string region = arg;

					if (region == "left") {
						PrismaUI->Focus(PView);
					} else if (region == "right") {
						if (!PrismaUI->IsHidden(ConfigView)) {
							PrismaUI->Focus(ConfigView);
						}
					} else if (region == "leave") {
						if (PrismaUI->IsHidden(ConfigView)) {
							PrismaUI->Unfocus(PView);
						}
					}
				});

				PrismaUI->RegisterJSListener(ConfigView, "btnAction", [](const char* arg) {
					std::string region = arg;

					if (region == "close") {
						PrismaUI->Focus(PView);
						PrismaUI->Hide(ConfigView);
					} 
				});


			} catch (const std::exception& e) {
				logger::info("exception : {}", e.what());
			}
		});

		PrismaUI->RegisterJSListener(PView, "autoFocusView", [](const char* arg) {
			std::string region = arg;

			if (region == "leave") {
				if (PrismaUI->IsHidden(ConfigView)) {
					PrismaUI->Unfocus(PView);
				}
			}
		});
    }

	void FocusReload() {
		PrismaUI->Invoke(PView, "focusReload()");
	}


	static std::string ReadAllText(const fs::path& p)
	{
		std::ifstream ifs(p, std::ios::binary);
		if (!ifs.is_open()) {
			return {};
		}

		std::string s;
		ifs.seekg(0, std::ios::end);
		s.resize(static_cast<size_t>(ifs.tellg()));
		ifs.seekg(0, std::ios::beg);
		ifs.read(s.data(), static_cast<std::streamsize>(s.size()));
		return s;
	}

	static bool IsValidJson(const std::string& s)
	{
		try {
			(void)nlohmann::json::parse(s);
			return true;
		} catch (...) {
			return false;
		}
	}

	std::string LoadPMCMJsons_CommaJoined()
	{
	    const fs::path folder = "Data/PrismaUI/PMCM";
	
	    std::vector<std::string> validJsonTexts;
	
	    std::error_code ec;
	
	    logger::info("[PMCM] scanning folder: {}", folder.string());
	
	    if (!fs::exists(folder, ec)) {
	        logger::info("[PMCM] folder NOT FOUND");
	        return "[]";
	    }
	
	    for (auto const& entry : fs::directory_iterator(folder, ec))
	    {
	        if (ec) break;
	        if (!entry.is_regular_file()) continue;
	
	        const fs::path p = entry.path();
	
	        if (p.extension() != ".txt")
	            continue;
	
	        std::string text = ReadAllText(p);
	        if (text.empty())
	            continue;
	
	        try
	        {
	            if (IsValidJson(text))
	            {
	                validJsonTexts.push_back(text);
	                logger::info("[PMCM] loaded: {}", p.filename().string());
	            }
	            else
	            {
	                logger::info("[PMCM] invalid json skipped: {}", p.filename().string());
	            }
	        }
	        catch (...)
	        {
	            logger::info("[PMCM] exception parsing file: {}", p.filename().string());
	        }
	    }
	
	    std::string out;
	
	    for (size_t i = 0; i < validJsonTexts.size(); ++i)
	    {
	        if (i > 0)
	            out += ",";
	        out += validJsonTexts[i];
	    }
	
	    if (out.empty())
	    {
	        logger::info("[PMCM] no valid entries found");
	        return "[]";
	    }
	
	    std::string result = "[" + out + "]";
	
	    logger::info("[PMCM] FINAL JSON READY");
	
	    return result;
	}

	void passConfig()
	{
		std::thread([] {
			std::this_thread::sleep_for(std::chrono::milliseconds(6000));

			SKSE::GetTaskInterface()->AddTask([] {
				menuJson = LoadPMCMJsons_CommaJoined();
				//logger::info("menuJson : {} ", menuJson);

				std::string script = "menuInit(" + menuJson + ")";
				PrismaUI->Invoke(PView, script.c_str());
			});
		}).detach();
	}

	void ResetMenu() {
		PrismaUI->InteropCall(ConfigView, "ResetMenu", "0");

		std::thread([] {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			SKSE::GetTaskInterface()->AddTask([] {
				PrismaUI->Hide(PView);
				PrismaUI->Hide(ConfigView);
			});
		}).detach();
	}
	
};
