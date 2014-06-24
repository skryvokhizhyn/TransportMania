#ifndef _BMPHANDLER_H_
#define _BMPHANDLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace utils
{
	class BmpHandler
		: boost::noncopyable
	{
	public:
		typedef unsigned char ValueType;
		typedef std::vector<ValueType> ValueBuffer;

	public:
		BmpHandler(const std::string & file);

		void Get(const size_t x, const size_t y, ValueType * pBuff, const size_t cnt) const;
		void Put(const size_t x, const size_t y, const ValueType * pBuff, const size_t cnt);

		size_t GetWidth() const;
		size_t GetHeight() const;

	private:
		size_t GetPositionOffset(const size_t x, const size_t y) const;

	private:
		typedef std::unique_ptr<FILE, int (*)(FILE *)> FilePtr;

	private:
		size_t width_;
		size_t rowSize_;
		size_t height_;
		size_t offset_;
		FilePtr filePtr_;
	};

} // namespace utils

#endif // _BMPHANDLER_H_