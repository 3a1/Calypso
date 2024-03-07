#pragma once
#include "mainLoop.h"
#include "functions.h"


namespace Mouse {
	namespace Aimbot {
		void start(Loop& loopInstance, int AimSpeed, int SmoothAmount, int fov, int button);
	}

	namespace Triggerbot {
		void start(Loop& loopInstance, int triggerbot_delay_b, int triggerbot_delay_a, int button);
	}
}

namespace Arduino {
	namespace Aimbot {
		void start(Loop& loopInstance, int AimSpeed, int SmoothAmount, int fov, int button);
	}

	namespace Triggerbot {
		void start(Loop& loopInstance, int triggerbot_delay_b, int triggerbot_delay_a, int button);
	}
}