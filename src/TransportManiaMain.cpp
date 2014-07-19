#include "Game.h"
#include "SafeExecute.h"

int main(int /*argc*/, char ** /*argv*/)
{
	const bool res = utils::SafeExecuteWithLog(
	[]()
	{
		trm::Game game;
		game.Run();
	});

	return res ? 0 : 1;
}
