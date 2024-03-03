#pragma once
#include "mainLoop.h"
#include "functions.h"

void mouseAimbot(Loop& loopInstance, int AimSpeed, int SmoothAmount, int fov, int button);
void mouseTriggerbot(Loop& loopInstance, int triggerbot_delay_b, int triggerbot_delay_a, int button);