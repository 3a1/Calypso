
#include "includes.h"
#include "struct.h"
#include "offsets.h"
#include "arduino.h"

struct cord {
    float x = 0;
    float y = 0;
};

void AimAtPos(float x, float y, int AimSpeed, int SmoothAmount)
{
    float TargetX = 0;
    float TargetY = 0;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int ScreenCenterX = cursorPos.x;
    int ScreenCenterY = cursorPos.y;

    bool Smooth = true;

    if (x != 0)
    {
        if (x > ScreenCenterX)
        {
            TargetX = -(ScreenCenterX - x);
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
        }

        if (x < ScreenCenterX)
        {
            TargetX = x - ScreenCenterX;
            TargetX /= AimSpeed;
            if (TargetX + ScreenCenterX < 0) TargetX = 0;
        }
    }
    if (y != 0)
    {
        if (y > ScreenCenterY)
        {
            TargetY = -(ScreenCenterY - y);
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
        }

        if (y < ScreenCenterY)
        {
            TargetY = y - ScreenCenterY;
            TargetY /= AimSpeed;
            if (TargetY + ScreenCenterY < 0) TargetY = 0;
        }
    }
    if (!Smooth)
    {
        mouse_event(0x0001, (UINT)(TargetX), (UINT)(TargetY), NULL, NULL);
        return;
    }
    else
    {
        TargetX /= SmoothAmount;
        TargetY /= SmoothAmount;
        if (abs(TargetX) < 1)
        {
            if (TargetX > 0)
            {
                TargetX = 1;
            }
            if (TargetX < 0)
            {
                TargetX = -1;
            }
        }
        if (abs(TargetY) < 1)
        {
            if (TargetY > 0)
            {
                TargetY = 1;
            }
            if (TargetY < 0)
            {
                TargetY = -1;
            }
        }
        mouse_event(0x0001, TargetX, TargetY, NULL, NULL);
        return;
    }
}

void AimbotMouse(DWORD pid, uintptr_t client, int AimSpeed, int SmoothAmount, int fov)
{

    while (true) {


        uintptr_t localPlayer = Driver::read<uintptr_t>(pid, client + offsets::dwLocalPlayerPawn);
        uintptr_t entity_list = Driver::read<uintptr_t>(pid, client + offsets::dwEntityList);

        if (GetAsyncKeyState(VK_LBUTTON)) {

            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);

            POINT cursorPos;
            GetCursorPos(&cursorPos);

            Vector3 Localorigin = Driver::read<Vector3>(pid, localPlayer + offsets::m_vecOrigin);
            view_matrix_t view_matrix = Driver::read<view_matrix_t>(pid, client + offsets::dwViewMatrix);
            int localTeam = Driver::read<int>(pid, localPlayer + offsets::m_iTeamNum);

            for (int i = 1; i < 32; ++i) {

                uintptr_t listEntry = Driver::read<uintptr_t>(pid, entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
                if (!listEntry) continue;
                uintptr_t player = Driver::read<uintptr_t>(pid, listEntry + 120 * (i & 0x1FF));
                if (!player) continue;
                uintptr_t playerPawn = Driver::read<uintptr_t>(pid, player + offsets::m_hPlayerPawn);
                uintptr_t listEntry2 = Driver::read<uintptr_t>(pid, entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
                if (!listEntry2) continue;

                uintptr_t pCSPlayerPawn = Driver::read<uintptr_t>(pid, listEntry2 + 120 * (playerPawn & 0x1FF));
                if (!pCSPlayerPawn) continue;
                if (pCSPlayerPawn == localPlayer) continue;

                int health = Driver::read<int>(pid, pCSPlayerPawn + offsets::m_iHealth);
                if (health <= 0 || health > 100) continue;

                Vector3 origin = Driver::read<Vector3>(pid, pCSPlayerPawn + offsets::m_vOldOrigin);
                Vector3 head = { origin.x, origin.y, origin.z + 65.f };

                Vector3 screenHead = head.WTS(view_matrix);

                if (screenHead.x == 0) {
                    continue;
                }

                if (screenHead.x > (screenWidth / 2) - fov && screenHead.x < (screenWidth / 2) + fov) {
                    if (screenHead.y > (screenHeight / 2) - fov && screenHead.y < (screenHeight / 2) + fov) {

                        AimAtPos(screenHead.x, screenHead.y, AimSpeed, SmoothAmount);
                        break;
                    }
                }

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    }
    
}