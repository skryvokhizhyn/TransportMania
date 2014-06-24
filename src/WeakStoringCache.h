#pragma once

#include <memory>
#include <map>

namespace trm
{
	template<typename Key, typename Value>
	class WeakStoringCache
	{
	public:
		typedef std::shared_ptr<Value> Ptr;

		enum Type
		{
			Missing,
			Active,
			Cleared
		};

	public:
		Ptr Get(const Key & k);
		Type Check(const Key & k);

	private:
		typedef std::weak_ptr<Value> WPtr;
		typedef std::map<Key, WPtr> Storage;

	private:
		Storage storage_;
	};

	template<typename Key, typename Value>
	auto WeakStoringCache<Key, Value>::Get(const Key & k) -> Ptr
	{
		const auto found = storage_.find(k);

		Ptr ret;

		if (found == storage_.end())
		{
			ret.reset(new Value());
			storage_.insert(std::make_pair(k, WPtr(ret)));
		}
		else
		{
			WPtr wptr = found->second;

			if (!(ret = wptr.lock()))
			{
				ret.reset(new Value());
				found->second = WPtr(ret);
			}
		}

		return ret;
	}

	template<typename Key, typename Value>
	auto WeakStoringCache<Key, Value>::Check(const Key & k) -> Type
	{
		const auto found = storage_.find(k);

		if (found == storage_.end())
		{
			return Type::Missing;
		}

		if (found->second.expired())
		{
			return Type::Cleared;
		}

		return Type::Active;
	}

} // namespace trm
