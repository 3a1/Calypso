#include "features.h"

void aimbot::start() 
{
    while (true)
    {
        if (GetAsyncKeyState(config::aimbot.key) || config::aimbot.enable_b)
        {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            int fovHalf = config::aimbot.fov / 2;
            int lowerX = centerX - fovHalf;
            int upperX = centerX + fovHalf;
            int lowerY = centerY - fovHalf;
            int upperY = centerY + fovHalf;

            playerPosition** ppPositions = Loop::loop.getPlayerPositions();
            double closestDistance = 999999999.0;
            int closestX = 0;
            int closestY = 0;

            for (int i = 0; i < 32; ++i)
            {
                int x = ppPositions[i]->screenPosition[0];
                int y = ppPositions[i]->screenPosition[1];

                if (x > lowerX && x < upperX && y > lowerY && y < upperY)
                {
                    double dist = utils::distance(centerX, centerY, x, y);
                    if (dist < closestDistance)
                    {
                        closestDistance = dist;
                        closestX = x;
                        closestY = y;
                    }
                }
            }

            if (closestDistance < 999999999.0)
            {
                config::arduino.enable ? utils::aimAtPosArduino(closestX, closestY) : utils::aimAtPos(closestX, closestY);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void triggerbot::start() 
{
    while (true) 
    {
        if (GetAsyncKeyState(config::triggerbot.key)) 
        {
            if (config::misc.tg_with_ab) config::aimbot.enable_b = true;
            if (Loop::loop.isPlayerOnCrosshair()) 
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(config::triggerbot.delay_b));
                config::arduino.enable ? utils::leftClickArduino() : utils::leftClick();
                std::this_thread::sleep_for(std::chrono::milliseconds(config::triggerbot.delay_a));
            }
        }
        else if (config::aimbot.enable_b)
            config::aimbot.enable_b = false;
    }
}

void features::start() 
{
    if (config::triggerbot.enable && config::aimbot.enable)
    {
        std::thread triggerbotThread(triggerbot::start);
        std::thread aimbotThread(aimbot::start);
        utils::printc("[+]", "Aimbot & Triggerbot threads started", GREEN);
        aimbotThread.join();
    }
    else if (config::aimbot.enable)
    {
        std::thread aimbotThread(aimbot::start);
        utils::printc("[+]", "Aimbot thread started", GREEN);
        aimbotThread.join();
    }
    else if (config::triggerbot.enable)
    {
        std::thread triggerbotThread(triggerbot::start);
        utils::printc("[+]", "Triggerbot thread started", GREEN);
        triggerbotThread.join();
    }
}