#pragma once
#include "includes.h"
#include "struct.h"
#include "offsets.h"
#include "utils.h"
#include "config.h"
#include "driver.h"

class Loop
{
public:
    static Loop loop;

    bool entityOnCrosshair;
    DWORD pid;
    uintptr_t client;
    playerPosition* ppPos[32];

    Loop();
    ~Loop();
    void updateLoop();
    bool isPlayerOnCrosshair();
    playerPosition** getPlayerPositions();
};