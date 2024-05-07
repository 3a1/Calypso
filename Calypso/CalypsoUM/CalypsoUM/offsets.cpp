#include "offsets.h"

namespace offsets
{
    // these offsets below are almost always change after a game update
    uintptr_t dwEntityList = 0;
    uintptr_t dwLocalPlayerPawn = 0;
    uintptr_t dwViewMatrix = 0;

    // these offsets below are often the same after update
    uintptr_t m_iIDEntIndex = 0;
    uintptr_t m_hPlayerPawn = 0;
    uintptr_t m_iTeamNum = 0;
    uintptr_t m_iHealth = 0;
    uintptr_t m_vecOrigin = 0;

    uintptr_t m_entitySpottedState = 0;
    uintptr_t m_bSpottedByMask = 0;
}

uintptr_t readOffset(const std::string& name, const std::string& struct_name, const std::string& path) 
{
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        Log("[-]", "Failed to open offsets file: " + path, RED);
        Log("[!]", "Please open offsets_update.py and restart cheat.", YELLOW);
        system("pause");
        exit(1);
    }

    std::string line;
    unsigned int value = 0;
    size_t temp;

    bool structFinded = false;

    while (std::getline(file, line)) 
    {
        if (line.empty())
            continue;
        if (!structFinded) 
        {
            temp = line.find(struct_name + "\": {");
            if (temp == std::string::npos)
                continue;
            else
                structFinded = true;
        }

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            continue;

        std::string key = line.substr(0, colonPos);
        size_t startPos = line.find_first_of("0123456789", colonPos);
        if (startPos == std::string::npos)
            continue;

        size_t endPos = line.find_last_of("0123456789");
        if (endPos == std::string::npos)
            continue;

        std::string valueStr = line.substr(startPos, endPos - startPos + 1);
        unsigned int readValue = std::stoi(valueStr);

        if (key.find(name) != std::string::npos)
        {
            value = readValue;
            break;
        }
    }
    file.close();

    std::stringstream ss;
    ss << std::hex << value; // Convert value to hexadecimal
    uintptr_t result;
    ss >> result;
    return result;
}

bool readOffsets() 
{
    offsets::dwEntityList = readOffset("dwEntityList", "client.dll", "offsets/offsets.json");
    offsets::dwLocalPlayerPawn = readOffset("dwLocalPlayerPawn", "client.dll", "offsets/offsets.json");
    offsets::dwViewMatrix = readOffset("dwViewMatrix", "client.dll", "offsets/offsets.json");

    offsets::m_iIDEntIndex = readOffset("m_iIDEntIndex", "C_CSPlayerPawnBase", "offsets/client.dll.json");
    offsets::m_hPlayerPawn = readOffset("m_hPlayerPawn", "CCSPlayerController", "offsets/client.dll.json");
    offsets::m_iTeamNum = readOffset("m_iTeamNum", "C_BaseEntity", "offsets/client.dll.json");
    offsets::m_iHealth = readOffset("m_iHealth", "C_BaseEntity", "offsets/client.dll.json");
    offsets::m_vecOrigin = readOffset("m_vecOrigin", "CGameSceneNode", "offsets/client.dll.json");

    offsets::m_entitySpottedState = readOffset("m_entitySpottedState", "C_CSPlayerPawn", "offsets/client.dll.json");
    offsets::m_bSpottedByMask = readOffset("m_bSpottedByMask", "EntitySpottedState_t", "offsets/client.dll.json");

    if (offsets::dwEntityList == 0 || offsets::dwLocalPlayerPawn == 0 || offsets::dwViewMatrix == 0 ||
        offsets::m_iIDEntIndex == 0 || offsets::m_hPlayerPawn == 0 || offsets::m_iTeamNum == 0 ||
        offsets::m_iHealth == 0 || offsets::m_vecOrigin == 0)
        return false;
    else
        return true;
}