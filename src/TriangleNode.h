#ifndef _TRIANGLENODE_H_
#define _TRIANGLENODE_H_

#include "Types.h"

namespace trm
{
	struct Triangle3d;

namespace terrain
{
	class HeightMap;

namespace lod
{
	class Variance;

	class TriangleNode
	{
	public:
		enum class ProcessBase : bool
		{
			Merge,
			Ignore
		};

	public:
		explicit TriangleNode(const size_t num = 1);
		~TriangleNode();

		void SetBase(TriangleNode * pBase);
		void SetLNeighbor(TriangleNode * pLNeighbor);
		void SetRNeighbor(TriangleNode * pRNeighbor);
				
		bool IsValid() const;

		void SetClearCause(const bool c, const bool recursive);
		
		bool Splitted() const;
		void Split();
		bool Merge(const ProcessBase pb);

		TriangleNode * GetLChild() const;
		TriangleNode * GetRChild() const;
		size_t GetNum() const;

	public:
		void * operator new (size_t sz);
		void operator delete (void * p);

	private:
		void * operator new (size_t sz, const std::nothrow_t &);
		void * operator new[] (size_t sz);
		void operator delete[] (void * p);

	private:
		void BindSplit();
		void BindSplitNeighbor(TriangleNode * pNeighbor, TriangleNode * pChild);
		void BindMerge();
		void BindMergeNeighbor(TriangleNode * pNeighbor, TriangleNode * pChild);

	private:
		TriangleNode * pLChild_; 
		TriangleNode * pRChild_; 
		TriangleNode * pBase_; 
		TriangleNode * pLNeighbor_; 
		TriangleNode * pRNeighbor_;
		TriangleNode * pParent_;
		size_t num_;
		bool causedSplitting_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm

#endif // _TRIANGLENODE_H_