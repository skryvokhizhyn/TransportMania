#pragma once

#include <memory>

namespace utils
{
	template<typename Type, typename ... Args>
	std::unique_ptr<Type> MakeUniquePtr(Args && ... args)
	{
		return std::unique_ptr<Type>(new Type(std::forward<Args>(args)...));
	}

} // namespace utils

