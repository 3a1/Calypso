#pragma once
#include <sstream>

namespace offsets { // client.dll
    constexpr std::ptrdiff_t dwEntityList = 0x18BBAF8;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x1730118;
    constexpr std::ptrdiff_t dwViewMatrix = 0x191CF30;
    constexpr std::ptrdiff_t dwViewAngles = 0x1929730;

    constexpr std::ptrdiff_t m_iIDEntIndex = 0x15A4;

    constexpr std::ptrdiff_t m_vecViewOffset = 0xC58;
    constexpr std::ptrdiff_t m_hPlayerPawn = 0x7E4;
    constexpr std::ptrdiff_t m_lifeState = 0x338;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3CB;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x127C;
    constexpr std::ptrdiff_t m_iHealth = 0x334;
    constexpr std::ptrdiff_t m_vecOrigin = 0x550;

}