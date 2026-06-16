#include "PMCM.h"
#include <DKUtil/Logger.hpp>
#include <thread>
#include "APIHandler.h"

//#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\TK Dodge RE.ini"


void Settings::ReadKeyCodeSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = static_cast<int>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}
void Settings::ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = static_cast<int>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}
void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}

void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
}



void Settings::readSettings()
{
	INFO("Reading Settings...");
	CSimpleIniA ini;
	//ini.LoadFile(SETTINGFILE_PATH);

	//ReadKeyCodeSetting(ini, "Main", "DodgeHotkey", FocusHotKey);

	INFO("...done");
}


/* bool Settings::SaveSettings(const Settings& s)
{
	CSimpleIniA ini;
	ini.SetUnicode();
//	ini.LoadFile(SETTINGFILE_PATH);
	// Main
	//ini.SetLongValue("Main", "DodgeHotkey", s.FocusHotKey);
	
//	return ini.SaveFile(SETTINGFILE_PATH) >= 0;
}*/