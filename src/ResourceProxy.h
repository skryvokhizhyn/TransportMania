#pragma once

#include <memory>

namespace utils
{
	template<typename T>
	struct StaticHoldingStrategy
	{
		template<typename... Arg>
		static void Init(Arg... arg) { obj_.reset(new T(std::forward<Arg>(arg)...)); }
		static void Term() { obj_.reset(); }

	protected:
		T * Get() { return obj_.get(); }
		const T * Get() const { return obj_.get(); }

	private:
		static std::unique_ptr<T> obj_;
	};

	template<typename T>
	std::unique_ptr<T> StaticHoldingStrategy<T>::obj_;

	template<typename T>
	struct ByValueHoldingStrategy
	{
	public:
		template<typename... Arg>
		ByValueHoldingStrategy(Arg... arg)
			: obj_(std::forward<Arg>(arg)...)
		{}
	
	protected:
		T * Get() { return &obj_; }
		const T * Get() const { return &obj_; }

	private:
		T obj_;
	};

	template<typename T>
	struct StaticRefHoldingStrategy
	{
		static void Init(T & obj) { obj_ = &obj; }
		static void Term() { obj_ = nullptr; }

	protected:
		T * Get() 
		{ 
			assert(obj_ != nullptr);
			return obj_; 
		}
		const T * Get() const 
		{ 
			assert(obj_ != nullptr);
			return obj_; 
		}

	private:
		static T * obj_;
	};

	template<typename T>
	T * StaticRefHoldingStrategy<T>::obj_ = nullptr;

	template<
		typename T
		, template <class> class HoldingStrategy>
	class ResourceProxy
		: public HoldingStrategy<T>
	{
	public:
		template<typename... Args>
		ResourceProxy(Args... args)
			: HoldingStrategy<T>(std::forward<Args>(args)...)
		{}

		T * operator -> () { return HoldingStrategy<T>::Get(); }
		const T * operator -> () const { return HoldingStrategy<T>::Get(); }
	};

} // namespace utils
