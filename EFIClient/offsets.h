#pragma once
#include <sstream>

namespace offsets
{
    // these offsets below are almost always change after a game update
    constexpr std::ptrdiff_t dwEntityList = 0x18C1DB8;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17361E8;
    constexpr std::ptrdiff_t dwViewMatrix = 0x19231B0;
    constexpr std::ptrdiff_t dwViewAngles = 0x192F940;

    // these offsets below are often the same after update
    constexpr std::ptrdiff_t m_iIDEntIndex = 0x15A4;
    constexpr std::ptrdiff_t m_vecViewOffset = 0xC58;
    constexpr std::ptrdiff_t m_hPlayerPawn = 0x7E4;
    constexpr std::ptrdiff_t m_lifeState = 0x338;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3CB;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x127C;
    constexpr std::ptrdiff_t m_iHealth = 0x334;
    constexpr std::ptrdiff_t m_vecOrigin = 0x550; // there are two offsets in offset dumper for m_vecOrigin, we need the second one (not 0x80)
}