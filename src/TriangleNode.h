#pragma once

#include "Types.h"

#include <boost/noncopyable.hpp>

namespace trm
{
namespace terrain
{
namespace lod
{
	class TriangleNode;

	using TriangleNodePtr = TriangleNode *;

	class TriangleNode
		: boost::noncopyable
	{
	public:
		enum class ProcessBase : bool
		{
			Merge,
			Ignore
		};

		enum class RemoveAction : bool
		{
			Delete,
			Mark
		};

		enum class RecursiveMode : bool
		{
			No,
			Yes
		};

	public:
		explicit TriangleNode(const size_t num = 1);
		~TriangleNode();
		
		void SetBase(TriangleNodePtr pBase);
		void SetLNeighbor(TriangleNodePtr pLNeighbor);
		void SetRNeighbor(TriangleNodePtr pRNeighbor);
				
		bool IsValid() const;

		void SetClearCause(const bool c, const RecursiveMode rm);
		void MarkForDelete(const bool v, const RecursiveMode rm);
		
		bool Splitted() const;
		void Split();
		bool Merge(const ProcessBase pb, const RemoveAction ra);
		void RemoveChildren();
	
		TriangleNodePtr GetParent() const;
		TriangleNodePtr GetLChild() const;
		TriangleNodePtr GetRChild() const;
		size_t GetNum() const;
		bool MarkedForDelete() const;

	public:
		static void * operator new (size_t sz);
		static void operator delete (void * p);
		static void * operator new (unsigned int, int, const char *, int);
		static void operator delete (void * p, int, const char *, int);

	private:
		static void * operator new (size_t sz, const std::nothrow_t &);
		static void * operator new[] (size_t sz);
		static void operator delete[] (void * p);

	private:
		void BindSplit();
		void BindSplitNeighbor(TriangleNodePtr pNeighbor, TriangleNodePtr pChild);
		void BindMerge();
		void BindMergeNeighbor(TriangleNodePtr pNeighbor, TriangleNodePtr pChild);
		void AllocateChildren();
		bool HasChildren() const;
		bool ChildrenMarkedForDelete() const;

		TriangleNodePtr GetLNeighbor() const;
		TriangleNodePtr GetRNeighbor() const;
		TriangleNodePtr GetBase() const;

	private:
		TriangleNodePtr pLChild_; 
		TriangleNodePtr pRChild_; 
		TriangleNodePtr pBase_; 
		TriangleNodePtr pLNeighbor_; 
		TriangleNodePtr pRNeighbor_;
		TriangleNodePtr pParent_;
		size_t num_;
		bool causedSplitting_;
		bool markedForDelete_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm
