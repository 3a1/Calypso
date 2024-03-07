#pragma once
#include "mainLoop.h"
#include "functions.h"

namespace Mouse {
	namespace Aimbot {
		void start(Loop& loopInstance);
	}

	namespace Triggerbot {
		void start(Loop& loopInstance);
	}
}

namespace Arduino {
	namespace Aimbot {
		void start(Loop& loopInstance);
	}

	namespace Triggerbot {
		void start(Loop& loopInstance);
	}
}