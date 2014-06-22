#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <boost/function.hpp>
#include <sstream>

namespace utils
{
	class Logger
	{
	public:
		~Logger();

		std::stringstream & Info();
		std::stringstream & Warning();
		std::stringstream & Error();
		std::stringstream & Trace();
		std::stringstream & Debug();

	private:
		typedef boost::function<void (const std::stringstream &)> OutputFunction;

	private:
		std::stringstream str_;
		OutputFunction func_;
	};

} // namespace utils

#endif // _LOGGER_H_