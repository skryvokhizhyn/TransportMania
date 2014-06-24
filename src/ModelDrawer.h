#ifndef _MODELDRAWER_H_
#define _MODELDRAWER_H_

#include "Types.h"
#include <boost/noncopyable.hpp>

namespace trm
{
	struct ModelData;

	class ModelDrawer
		: private boost::noncopyable
	{
	public:
		ModelDrawer();
		~ModelDrawer();

		void Load(const ModelData & md);
		void Draw() const;

	private:
		GLuintType buffVert_;
		GLuintType buffIndx_;
		GLuintType buffNorm_;
		size_t indxCnt_;
	};

} // namespace trm

#endif // _MODELDRAWER_H_