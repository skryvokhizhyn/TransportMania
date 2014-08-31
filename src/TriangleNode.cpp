#include "Point3d.h"
#include "TriangleNode.h"
#include "Triangle3d.h"
#include "Variance.h"
#include "HeightMap.h"
#include "GeometryUtils.h"
#include "CommonUtils.h"
#include "HeightMap.h"
#include "Logger.h"
#include "TriangleNodePool.h"

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

namespace
{
	bool Asking_For_Member_Check(bool markedForDete, TriangleNodePtr /*dummy*/) 
	{
		return !markedForDete;
	}
}

TriangleNode::TriangleNode(const size_t num /*= 1*/)
	: pLChild_(nullptr), pRChild_(nullptr)
	, pBase_(nullptr), pLNeighbor_(nullptr), pRNeighbor_(nullptr), pParent_(nullptr)
	, num_(num)
	, causedSplitting_(false)
	, markedForDelete_(false)
{
}

TriangleNode::~TriangleNode()
{
	RemoveChildren();
}

void *
TriangleNode::operator new (size_t /*sz*/)
{
	void * p = TriangleNodePool::Get<TriangleNode>().Malloc();
	
	if (p == nullptr)
	{
		throw std::bad_alloc();
	}

	return p;
}

void * 
TriangleNode::operator new (unsigned int sz, int, const char *, int)
{
	return TriangleNode::operator new (sz);
}

void
TriangleNode::operator delete (void * p)
{
	TriangleNodePool::Get<TriangleNode>().Free(p);
}

void
TriangleNode::operator delete (void * p, int, const char *, int)
{
	TriangleNode::operator delete(p);
}

bool
TriangleNode::HasChildren() const
{
	const bool hasChildren = (pLChild_ != nullptr && pRChild_ != nullptr);
	// assert that both children are nullptr
	assert(!hasChildren ? (pLChild_ == nullptr && pRChild_ == nullptr) : true);

	return hasChildren;
}

bool
TriangleNode::ChildrenMarkedForDelete() const
{
	assert(pLChild_);
	assert(pRChild_);

	const bool childrenMarkedForDelete = pLChild_->markedForDelete_ && pRChild_->markedForDelete_;
	// assert that both children not marked for delete
	assert(!childrenMarkedForDelete ? !(pLChild_->markedForDelete_ || pRChild_->markedForDelete_) : true);

	return childrenMarkedForDelete;
}

bool 
TriangleNode::Splitted() const
{
	const bool splitted = HasChildren();

	bool childrenMarkedForDelete = false;

	if (splitted)
	{
		childrenMarkedForDelete = ChildrenMarkedForDelete();
	}

	return splitted && !childrenMarkedForDelete;
}

void 
TriangleNode::Split()
{
	if (Splitted())
	{
		return;
	}

	AllocateChildren();

	if (pBase_ && !pBase_->Splitted())
	{
		pBase_->Split();

		// only possible to have 2 level difference
		if (!pBase_->Splitted())
		{
			pBase_->Split();
		}
	}

	BindSplit();
}

bool
TriangleNode::Merge(const ProcessBase pb, const RemoveAction ra)
{
	if (!Splitted())
	{
		return true;
	}

	if (!pLChild_->Merge(ProcessBase::Merge, ra))
	{
		return false;
	}
	
	if (!pRChild_->Merge(ProcessBase::Merge, ra))
	{
		return false;
	}

	if (causedSplitting_)
	{
		return false;
	}

	if (pb == ProcessBase::Merge && pBase_)
	{
		if (!pBase_->Merge(ProcessBase::Ignore, ra))
		{
			return false;
		}

		// don't merge parent as it results into infinite recursion
		// and in any case we cannot completely reverse splitting
		/*TriangleNode * pBaseParent = pBase_->pParent_;

		if (pBaseParent && !(pBaseParent->pLChild_->Splitted() || pBaseParent->pRChild_->Splitted()))
		{
			pBaseParent->Merge(ProcessCause::Preserve, ProcessBase::Merge);
		}*/
	}

	BindMerge();

	if (ra == RemoveAction::Delete)
	{
		RemoveChildren();
	}
	else
	{
		pLChild_->MarkForDelete(true, RecursiveMode::Yes);
		pRChild_->MarkForDelete(true, RecursiveMode::Yes);
	}

	return true;
}

void 
TriangleNode::BindSplit()
{
	if (pLNeighbor_)
	{
		BindSplitNeighbor(pLNeighbor_, pLChild_);
	}

	if (pRNeighbor_)
	{
		BindSplitNeighbor(pRNeighbor_, pRChild_);
	}

	if (pBase_)
	{
		pLChild_->pRNeighbor_ = pBase_->pRChild_;
		pRChild_->pLNeighbor_ = pBase_->pLChild_;
	}

	pLChild_->pBase_ = pLNeighbor_;
	pLChild_->pLNeighbor_ = pRChild_;

	pRChild_->pBase_ = pRNeighbor_;
	pRChild_->pRNeighbor_ = pLChild_;
}

void
TriangleNode::BindSplitNeighbor(TriangleNodePtr pNeighbor, TriangleNodePtr pChild)
{
	if (pNeighbor->pBase_ == this)
	{
		pNeighbor->pBase_ = pChild;

		if (pNeighbor->pParent_->pRNeighbor_ == this)
		{
			pNeighbor->pParent_->pRNeighbor_ = pChild;
		}
		else
		{
			assert(pNeighbor->pParent_->pLNeighbor_ == this);
			pNeighbor->pParent_->pLNeighbor_ = pChild;
		}
	}
	else if (pNeighbor->pLNeighbor_ == this)
	{
		pNeighbor->pLNeighbor_ = pChild;
	}
	else
	{
		assert(pNeighbor->pRNeighbor_ == this);
		pNeighbor->pRNeighbor_ = pChild;
	}
}

void
TriangleNode::BindMerge()
{
	if (pLNeighbor_)
	{
		BindMergeNeighbor(pLNeighbor_, pLChild_);
	}

	if (pRNeighbor_)
	{
		BindMergeNeighbor(pRNeighbor_, pRChild_);
	}
}

void 
TriangleNode::BindMergeNeighbor(TriangleNodePtr pNeighbor, TriangleNodePtr pChild)
{
	if (pNeighbor->pBase_ == pChild)
	{
		pNeighbor->pBase_ = this;

		TriangleNodePtr pNParent = pNeighbor->pParent_;
		
		if(pNParent->pLNeighbor_ == pChild)
		{
			pNParent->pLNeighbor_ = this;
		}
		else
		{
			assert(pNParent->pRNeighbor_ == pChild);
			pNParent->pRNeighbor_ = this;
		}
	}
	else if (pNeighbor->pLNeighbor_ == pChild)
	{
		pNeighbor->pLNeighbor_ = this;
	}
	else
	{
		assert(pNeighbor->pRNeighbor_ == pChild);
		pNeighbor->pRNeighbor_ = this;
	}
}

void
TriangleNode::SetBase(TriangleNodePtr pBase)
{
	pBase_ = pBase;
}

void 
TriangleNode::SetLNeighbor(TriangleNodePtr pLNeighbor)
{
	pLNeighbor_ = pLNeighbor;
}

void 
TriangleNode::SetRNeighbor(TriangleNodePtr pRNeighbor)
{
	pRNeighbor_ = pRNeighbor;
}

bool
TriangleNode::IsValid() const
{
	bool valid = true;

	if (Splitted())
	{
		if (pBase_ && !pBase_->Splitted())
		{
			utils::Logger().Debug() << "Invalid! Desync with base. Curr [" << num_ << "] base [" << pBase_->num_ << "]";

			valid = false;
		}

		if (!pLChild_)
		{
			utils::Logger().Debug() << "Invalid! No LChild. Curr [" << num_ << "]";
		}

		if (!pRChild_)
		{
			utils::Logger().Debug() << "Invalid! No RChild. Curr [" << num_ << "]";
		}

		if (pLChild_)
		{
			valid = pLChild_->IsValid();
		}

		if (pRChild_)
		{
			valid = pRChild_->IsValid();
		}
	}

	return valid;
}

void
TriangleNode::SetClearCause(const bool c, const RecursiveMode rm)
{
	causedSplitting_ = c;

	if (rm == RecursiveMode::Yes && HasChildren())
	{
		pLChild_->SetClearCause(c, rm);
		pRChild_->SetClearCause(c, rm);
	}
}

void
TriangleNode::MarkForDelete(const bool v, const RecursiveMode rm)
{
	markedForDelete_ = v;

	if (rm == RecursiveMode::Yes && Splitted())
	{
		pLChild_->MarkForDelete(v, rm);
		pRChild_->MarkForDelete(v, rm);
	}
}

TriangleNodePtr
TriangleNode::GetParent() const
{
	return pParent_;
}

TriangleNodePtr
TriangleNode::GetLChild() const
{
	assert(Asking_For_Member_Check(markedForDelete_, pLChild_));
	return pLChild_;
}

TriangleNodePtr
TriangleNode::GetRChild() const
{
	assert(Asking_For_Member_Check(markedForDelete_, pRChild_));
	return pRChild_;
}

TriangleNodePtr 
TriangleNode::GetLNeighbor() const
{
	assert(Asking_For_Member_Check(markedForDelete_, pLNeighbor_));
	return pLNeighbor_;
}

TriangleNodePtr 
TriangleNode::GetRNeighbor() const
{
	assert(Asking_For_Member_Check(markedForDelete_, pRNeighbor_));
	return pRNeighbor_;
}
		
TriangleNodePtr 
TriangleNode::GetBase() const
{
	assert(Asking_For_Member_Check(markedForDelete_, pBase_));
	return pBase_;
}

size_t
TriangleNode::GetNum() const
{
	return num_;
}

bool 
TriangleNode::MarkedForDelete() const
{
	return markedForDelete_;
}

void 
TriangleNode::RemoveChildren()
{
	delete pLChild_;
	delete pRChild_;

	pLChild_ = nullptr;
	pRChild_ = nullptr;
}

void
TriangleNode::AllocateChildren()
{
	if (HasChildren())
	{
		if (ChildrenMarkedForDelete())
		{
			pLChild_->MarkForDelete(false, RecursiveMode::No);
			pRChild_->MarkForDelete(false, RecursiveMode::No);
		}
		else
		{
			// do nothing, although shouldn't happen
		}
	}
	else
	{
		pLChild_ = new TriangleNode(num_ * 2);
		pRChild_ = new TriangleNode(num_ * 2 + 1);

		pLChild_->pParent_ = this;
		pRChild_->pParent_ = this;
	}
}
