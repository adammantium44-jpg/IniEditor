#include "PCH.h"
#include "PrismaConfig.h"

#include <fstream>
#include <filesystem>

void WritePMCMConfig()
{
    std::filesystem::create_directories(
        "Data/PrismaUI/PMCM"
    );

    std::ofstream file(
        "Data/PrismaUI/PMCM/EnxyAbilities.txt"
    );

    file <<
R"({
    "desc":"Enxy abilities settings",
    "id":"EnxyAbilities",
    "name":"EnxyAbilities",
    "pid":"6401906875496654797"
})";

    file.close();
}
