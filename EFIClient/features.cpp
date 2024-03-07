#include "features.h"


namespace Mouse {
    namespace Aimbot {
        void start(Loop& loopInstance, int AimSpeed, int SmoothAmount, int fov, int button) {
            while (true) {

                if (GetAsyncKeyState(button)) {
                    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                    int centerX = screenWidth / 2;
                    int centerY = screenHeight / 2;
                    int fovHalf = fov / 2;
                    int lowerX = centerX - fovHalf;
                    int upperX = centerX + fovHalf;
                    int lowerY = centerY - fovHalf;
                    int upperY = centerY + fovHalf;

                    PlayerPosition** ppPositions = loopInstance.getPlayerPositions();
                    double closestDistance = 999999999.0;
                    int closestX = 0;
                    int closestY = 0;

                    for (int i = 0; i < 32; ++i) {
                        int x = ppPositions[i]->screenPosition[0];
                        int y = ppPositions[i]->screenPosition[1];

                        if (x > lowerX && x < upperX && y > lowerY && y < upperY) {
                            double dist = distance(centerX, centerY, x, y);
                            if (dist < closestDistance) {
                                closestDistance = dist;
                                closestX = x;
                                closestY = y;
                            }
                        }
                    }

                    if (closestDistance < 999999999.0) {
                        AimAtPos(closestX, closestY, AimSpeed, SmoothAmount);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    namespace Triggerbot {
        void start(Loop& loopInstance, int triggerbot_delay_b, int triggerbot_delay_a, int button) {
            while (true) {
                if (GetAsyncKeyState(button)) {
                    if (loopInstance.isPlayerOnCrosshair()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_b));
                        LeftClick();
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_a));
                    }
                }

            }
        }
    }
}


namespace Arduino {
    namespace Aimbot {
        void start(Loop& loopInstance, int AimSpeed, int SmoothAmount, int fov, int button) {
            while (true) {

                if (GetAsyncKeyState(button)) {
                    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                    int centerX = screenWidth / 2;
                    int centerY = screenHeight / 2;
                    int fovHalf = fov / 2;
                    int lowerX = centerX - fovHalf;
                    int upperX = centerX + fovHalf;
                    int lowerY = centerY - fovHalf;
                    int upperY = centerY + fovHalf;

                    PlayerPosition** ppPositions = loopInstance.getPlayerPositions();
                    double closestDistance = 999999999.0;
                    int closestX = 0;
                    int closestY = 0;

                    for (int i = 0; i < 32; ++i) {
                        int x = ppPositions[i]->screenPosition[0];
                        int y = ppPositions[i]->screenPosition[1];

                        if (x > lowerX && x < upperX && y > lowerY && y < upperY) {
                            double dist = distance(centerX, centerY, x, y);
                            if (dist < closestDistance) {
                                closestDistance = dist;
                                closestX = x;
                                closestY = y;
                            }
                        }
                    }

                    if (closestDistance < 999999999.0) {
                        AimAtPosArduino(closestX, closestY, AimSpeed, SmoothAmount);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    namespace Triggerbot {
        void start(Loop& loopInstance, int triggerbot_delay_b, int triggerbot_delay_a, int button) {
            while (true) {
                if (GetAsyncKeyState(button)) {
                    if (loopInstance.isPlayerOnCrosshair()) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_b));
                        LeftClickArduino();
                        std::this_thread::sleep_for(std::chrono::milliseconds(triggerbot_delay_a));
                    }
                }

            }
        }
    }
}