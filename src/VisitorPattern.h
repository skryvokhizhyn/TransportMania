#pragma once

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/noncopyable.hpp>

namespace utils
{
namespace visitor
{
	template<typename T>
	struct VisitorNode
	{
		virtual void Visit(T & t) = 0;
	};

	template<typename TypesList>
	struct Visitor
		: boost::mpl::inherit_linearly<TypesList, boost::mpl::inherit<boost::mpl::_1, VisitorNode<boost::mpl::_2>>>::type
		, boost::noncopyable
	{
		virtual ~Visitor() {}
	};
	
	template<typename Base, typename ThisType, typename VisitorType>
	struct Acceptor
		: Base
	{
		virtual void Accept(VisitorType & v) override
		{
			static_cast<VisitorNode<ThisType> &>(v).Visit(static_cast<ThisType &>(*this));
		}
	};

} // namespace visitor
} // namespace utils
