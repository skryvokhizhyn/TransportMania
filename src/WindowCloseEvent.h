#pragma once

namespace trm
{
	class WindowCloseEvent
	{
	public:
		WindowCloseEvent(int id);

		void operator () () const;

	private:
		int id_;
	};

} // namespace trm
