#include <Logger.h>
#include <android/log.h>
#include <iostream>

using namespace utils;

namespace
{
	void PutInfo(const std::stringstream & str)
	{
		__android_log_print(ANDROID_LOG_INFO, "TM", "%s", str.str().c_str());
	}

	void PutWarning(const std::stringstream & str)
	{
		__android_log_print(ANDROID_LOG_WARN, "TM", "%s", str.str().c_str());
	}

	void PutError(const std::stringstream & str)
	{
		__android_log_print(ANDROID_LOG_ERROR, "TM", "%s", str.str().c_str());
	}

	void PutTrace(const std::stringstream & str)
	{
		__android_log_print(ANDROID_LOG_VERBOSE, "TM", "%s", str.str().c_str());
	}

	void PutDebug(const std::stringstream & str)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "TM", "%s", str.str().c_str());
	}
}

std::stringstream & 
Logger::Info()
{
	func_ = PutInfo;
	str_ << "[INF] ";
	return str_;
}

std::stringstream & 
Logger::Warning()
{
	func_ = PutWarning;
	str_ << "[WRN] ";
	return str_;
}

std::stringstream & 
Logger::Error()
{
	func_ = PutError;
	str_ << "[ERR] ";
	return str_;
}

std::stringstream & 
Logger::Trace()
{
	func_ = PutTrace;
	str_ << "[TRC] ";
	return str_;
}

std::stringstream & 
Logger::Debug()
{
	func_ = PutDebug;
	str_ << "[DBG] ";
	return str_;
}

Logger::~Logger()
{
	str_ << std::endl;
	func_(str_);
}
