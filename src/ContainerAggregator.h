#pragma once

#include <boost/ref.hpp>
#include <boost/circular_buffer.hpp>

#include <cstdint>

namespace utils
{
	namespace priv
	{
		template<typename Callback, typename Container, typename Value, int Size>
		struct ArgumentUnpacker
		{
			static void Apply(Callback & cb, const Container & ct);
		};
		
		template<typename Callback, typename Container, typename Value>
		struct ArgumentUnpacker<Callback, Container, Value, 2>
		{
			static void Apply(Callback & cb, const Container & ct)
			{
				cb(ct.at(0), ct.at(1));
			}
		};

		template<typename Callback, typename Container, typename Value>
		struct ArgumentUnpacker<Callback, Container, Value, 3>
		{
			static void Apply(Callback & cb, const Container & ct)
			{
				cb(ct.at(0), ct.at(1), ct.at(2));
			}
		};

} // namespace priv

	template<typename Callback, typename Value, std::uint8_t Size>
	class ContainerAggregator
	{
		using Buffer = boost::circular_buffer<Value>;

	public:
		ContainerAggregator(Callback & c)
			: callback_(c)
			, buff_(Size)
		{}

		void operator () (const Value & val)
		{
			buff_.push_back(val);

			if (buff_.full())
			{
				priv::ArgumentUnpacker<Callback, Buffer, typename Buffer::value_type, Size>::Apply(callback_, buff_);
				buff_.clear();
			}
		}

	private:
		using CallbackRef = boost::reference_wrapper<Callback>;

	private:
		Buffer buff_;
		CallbackRef callback_;
	};

} // namespace utils
