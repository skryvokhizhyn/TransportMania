#pragma once

#include <map>

namespace trm
{
	template<typename Key, typename Value>
	class SymmetricMap
	{
	public:
		typedef std::pair<Key,Key> KeyPair;
		typedef std::pair<Value, bool> ValueType;
		typedef std::map<KeyPair, ValueType> MapType;

	public:
		bool Insert(const KeyPair & k, Value);
		bool Erase(const KeyPair & k);
		bool Find(const KeyPair & k, Value & v, bool & d) const;
		bool Exists(const KeyPair & k) const;
		
		bool Normalized(const KeyPair & k) const;

		void Clear();

		const MapType & Data() const;

	private:
		KeyPair NormalizePair(const KeyPair & k) const;

	private:
		MapType map_;
	};

	template<typename Key, typename Value>
	bool SymmetricMap<Key, Value>::Insert(const KeyPair & k, Value v)
	{
		const KeyPair p = NormalizePair(k);
		bool direct = true;
		if (p != k)
		{
			direct = false;
		}

		return map_.emplace(std::move(p), std::make_pair(std::move(v), direct)).second;
	}

	template<typename Key, typename Value>
	bool SymmetricMap<Key, Value>::Erase(const KeyPair & k)
	{
		const KeyPair p = NormalizePair(k);

		return map_.erase(p) > 0;
	}

	template<typename Key, typename Value>
	bool SymmetricMap<Key, Value>::Normalized(const KeyPair & k) const
	{
		return (k.first < k.second);
	}

	template<typename Key, typename Value>
	auto SymmetricMap<Key, Value>::NormalizePair(const KeyPair & k) const -> KeyPair
	{
		if (Normalized(k))
		{
			return k;
		}
		else
		{
			return KeyPair(k.second, k.first);
		}
	}

	template<typename Key, typename Value>
	bool SymmetricMap<Key, Value>::Find(const KeyPair & k, Value & v, bool & d) const
	{
		const KeyPair p = NormalizePair(k);

		const auto found = map_.find(p);

		if (found == map_.end())
		{
			return false;
		}

		v = found->second.first;
		const bool b = found->second.second;
		d = (k == p) ? b : !b;

		return true;
	}

	template<typename Key, typename Value>
	bool SymmetricMap<Key, Value>::Exists(const KeyPair & k) const
	{
		const KeyPair p = NormalizePair(k);

		return map_.find(p) != map_.end();
	}

	/*template<typename Key, typename Value>
	void SymmetricMap<Key, Value>::CopyFrom(const SymmetricMap & other)
	{
		map_.insert(other.map_.begin(), other.map_.end());
	}*/

	template<typename Key, typename Value>
	void SymmetricMap<Key, Value>::Clear()
	{
		map_.clear();
	}

	template<typename Key, typename Value>
	auto SymmetricMap<Key, Value>::Data() const -> const MapType &
	{
		return map_;
	}

} // namespace trm
