#include "Game.h"
#include "PoolUsageCounterHolder.h"
#include "SafeExecute.h"
#include "LeakDetectorInitializer.h"

int main(int /*argc*/, char ** /*argv*/)
{
	const bool res = utils::SafeExecuteWithLog(
	[]()
	{
		utils::LeakDetectorInitializer dbgInit;

		{
			trm::Game game;
			game.Run();
		}

		utils::PoolUsageCounterHolder::Get().Validate();
	});

	return res ? 0 : 1;
}
