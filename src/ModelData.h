#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include "Types.h"
#include <map>

namespace trm
{
	struct ModelData
	{
		PointVector points;
		IndexVector indexes;
		NormaleVector normales;

		void Clear()
		{
			points.clear();
			indexes.clear();
			normales.clear();
		}

		bool Valid() const
		{
			return !points.empty();
		}
	};

} // namespace trm

#endif // _MODELDATA_H_