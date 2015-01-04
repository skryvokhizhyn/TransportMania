#pragma once

namespace trm
{
	class UniqueId
	{
	public:
		UniqueId();

		bool operator == (const UniqueId & other) const;
		bool operator < (const UniqueId & other) const;
		
	private:
		using IdType = unsigned long;

	private:
		IdType id_;
	};

} // namespace trm
