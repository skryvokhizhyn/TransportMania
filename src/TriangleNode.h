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

		enum class ProcessCause : bool
		{
			Preserve,
			Clear
		};

	public:
		explicit TriangleNode(const size_t num = 1);
		~TriangleNode();

		/*void Tasselate(const size_t level, const Variance & var,
			const HeightMap & hm, const Triangle3d & t, const Point3d & camera);*/
		/*void Render(const size_t level, const HeightMap & hm, const Triangle3d & t, 
			PointVector & result, IndexVector & index) const;*/
		
		void SetBase(TriangleNode * pBase);
		void SetLNeighbor(TriangleNode * pLNeighbor);
		void SetRNeighbor(TriangleNode * pRNeighbor);
				
		bool IsValid() const;

		void SetClearCause(const bool c, const bool recursive);
		
		bool Splitted() const;
		void Split();
		bool Merge(const ProcessCause pc, const ProcessBase pb);

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
		/*bool ShouldContinue(const Variance & var, 
			const Point3d & camera, const Triangle3d & t) const;*/

		//friend class TriangleNodeTester;

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