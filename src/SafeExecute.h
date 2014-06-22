#pragma once

#include "Logger.h"

namespace utils
{

	template<typename Func>
	bool SafeExecuteWithLog(Func && f)
	{
		try
		{
			f();

			return true;
		}
		catch (const std::exception & e)
		{
			Logger().Error() << "Exception: " << e.what();
		}
		catch (...)
		{
			Logger().Error() << "Unknown exception caught!";
		}

		return false;
	}

} // namespace utils
