#include "features.h"

void aimbot::run() 
{
    while (true)
    {
        if (GetAsyncKeyState(settings::aimbot::key) || shared::aimbot::enable_temp)
        {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            int fovHalf = settings::aimbot::fov / 2;
            int lowerX = centerX - fovHalf;
            int upperX = centerX + fovHalf;
            int lowerY = centerY - fovHalf;
            int upperY = centerY + fovHalf;

            double closestDistance = 999999999.0;
            int closestX = 0;
            int closestY = 0;

            for (auto& entity : entity_loop::player_list)
            {
                int x = entity.position.x;
                int y = entity.position.y;

                if (x > lowerX && x < upperX && y > lowerY && y < upperY)
                {
                    double dist = utils::math::distance(centerX, centerY, x, y);
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
                utils::mouse::aim_at(closestX, closestY);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void triggerbot::run() 
{
    while (true) 
    {
        if (GetAsyncKeyState(settings::triggerbot::key)) 
        {
            if (settings::misc::tg_with_ab) shared::aimbot::enable_temp = true;
            if (shared::triggetbot::onCrosshair) 
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(settings::triggerbot::delay_b));
                utils::mouse::left_click();
                std::this_thread::sleep_for(std::chrono::milliseconds(settings::triggerbot::delay_a));
            }
        }
        else if (shared::aimbot::enable_temp)
            shared::aimbot::enable_temp = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool features::start()
{
    if (settings::aimbot::enable)
    {
        std::thread aimbotThread(aimbot::run);
        aimbotThread.detach();
        Log("[+]", "Aimbot thread started", GREEN);
    }
    if (settings::triggerbot::enable)
    {
        std::thread triggerbotThread(triggerbot::run);
        triggerbotThread.detach();
        Log("[+]", "Triggerbot thread started", GREEN);
    }
    return (settings::triggerbot::enable || settings::aimbot::enable) ? true : false;
}