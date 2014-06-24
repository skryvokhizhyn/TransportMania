#ifndef _DRAWABLEOBJECT_H_
#define _DRAWABLEOBJECT_H_

#include <boost/noncopyable.hpp>
#include <memory>

namespace trm
{
	struct ModelData;

	struct DrawableObject
		: private boost::noncopyable
	{
		virtual ~DrawableObject() {}

		virtual void Render(ModelData & md) = 0;
	};

	typedef std::shared_ptr<DrawableObject> DrawableObjectPtr;

} // namespace trm

#endif // _DRAWABLEOBJECT_H_