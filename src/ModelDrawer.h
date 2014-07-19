#ifndef _MODELDRAWER_H_
#define _MODELDRAWER_H_

#include <memory>

namespace trm
{
	struct ModelData;

namespace impl
{
	struct State;

} // namespace impl

	class ModelDrawer
	{
	public:
		ModelDrawer();

		void Load(const ModelData & md);
		void Draw() const;

	private:
		using StatePtr = std::shared_ptr<impl::State>;

	private:
		StatePtr statePtr_;
	};

} // namespace trm

#endif // _MODELDRAWER_H_