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

TriangleNode::TriangleNode(const size_t num /*= 1*/)
	: pLChild_(NULL), pRChild_(NULL), pBase_(NULL), pLNeighbor_(NULL), pRNeighbor_(NULL)
	, causedSplitting_(false)
	, num_(num)
	, pParent_(NULL)
{
}

TriangleNode::~TriangleNode()
{
	delete pLChild_;
	delete pRChild_;
}

void *
TriangleNode::operator new (size_t /*sz*/)
{
	void * p = TriangleNodePool::Get().Malloc();
	
	if (p == nullptr)
	{
		throw std::bad_alloc();
	}

	return p;
}

void
TriangleNode::operator delete (void * p)
{
	TriangleNodePool::Get().Free(p);
}

bool 
TriangleNode::Splitted() const
{
	return (pLChild_ != NULL && pRChild_ != NULL);
}

void 
TriangleNode::Split()
{
	if (Splitted())
	{
		return;
	}

	pLChild_ = new TriangleNode(num_ * 2);
	pRChild_ = new TriangleNode(num_ * 2 + 1);

	pLChild_->pParent_ = this;
	pRChild_->pParent_ = this;

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
TriangleNode::Merge(const ProcessBase pb)
{
	if (!Splitted())
	{
		return true;
	}

	if (!pLChild_->Merge(ProcessBase::Merge))
	{
		return false;
	}
	
	if (!pRChild_->Merge(ProcessBase::Merge))
	{
		return false;
	}

	if (causedSplitting_)
	{
		return false;
	}

	if (pb == ProcessBase::Merge && pBase_)
	{
		if (!pBase_->Merge(ProcessBase::Ignore))
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

	delete pLChild_;
	delete pRChild_;

	pLChild_ = NULL;
	pRChild_ = NULL;

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
TriangleNode::BindSplitNeighbor(TriangleNode * pNeighbor, TriangleNode * pChild)
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
TriangleNode::BindMergeNeighbor(TriangleNode * pNeighbor, TriangleNode * pChild)
{
	if (pNeighbor->pBase_ == pChild)
	{
		pNeighbor->pBase_ = this;

		TriangleNode * pNParent = pNeighbor->pParent_;
		
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
TriangleNode::SetBase(TriangleNode * pBase)
{
	pBase_ = pBase;
}

void 
TriangleNode::SetLNeighbor(TriangleNode * pLNeighbor)
{
	pLNeighbor_ = pLNeighbor;
}

void 
TriangleNode::SetRNeighbor(TriangleNode * pRNeighbor)
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
TriangleNode::SetClearCause(const bool c, const bool recursive)
{
	causedSplitting_ = c;

	if (recursive && Splitted())
	{
		pLChild_->SetClearCause(c, recursive);
		pRChild_->SetClearCause(c, recursive);
	}
}

TriangleNode *
TriangleNode::GetLChild() const
{
	return pLChild_;
}

TriangleNode *
TriangleNode::GetRChild() const
{
	return pRChild_;
}

size_t
TriangleNode::GetNum() const
{
	return num_;
}