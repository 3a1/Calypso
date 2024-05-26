#pragma once
//#include "includes.h"
#include "utils.h"

bool readOffsets();

namespace offsets
{
    // these offsets below are almost always change after a game update
    extern uintptr_t dwEntityList;
    extern uintptr_t dwLocalPlayerPawn;
    extern uintptr_t dwViewMatrix;

    // these offsets below are often the same after update
    extern uintptr_t m_iIDEntIndex;
    extern uintptr_t m_hPlayerPawn;
    extern uintptr_t m_iTeamNum;
    extern uintptr_t m_iHealth;
    extern uintptr_t m_vecOrigin;
    extern uintptr_t m_vOldOrigin;
    extern uintptr_t m_pGameSceneNode;

    extern uintptr_t m_entitySpottedState;
    extern uintptr_t m_bSpottedByMask;

}