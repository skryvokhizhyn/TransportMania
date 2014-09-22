#include "ModelDrawer.h"
#include "Point3d.h"
#include "ModelData.h"
#include "Logger.h"
#include "DrawContext.h"
#include "GlobalDefines.h"
#include "Types.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <limits>

using namespace trm;

// we must preserve the size of Point3d as raw 3 * AxisType
// buffer creation relies on it!
static_assert(sizeof(Point3d) == 3 * sizeof(AxisType), "Expecting Point3d to be equal by size to 3 * size of AxisType");

namespace
{
	GLuint InitBuffer()
	{
		GLuint buffId = 0;
		glGenBuffers(1, &buffId);
		return buffId;
	}

	template<typename T>
	void LoadBuffer(const std::vector<T> & vec, const GLuint buffId, const GLuint buffType)
	{
		glBindBuffer(buffType, buffId);
		glBufferData(buffType, vec.size() * sizeof(T), &vec.at(0), GL_STATIC_DRAW);
		glBindBuffer(buffType, 0);
	}

	GLuintType ConvertDrawMode(const ModelData::Mode type)
	{
		switch(type)
		{
		case ModelData::Mode::Point:
			return GL_POINTS;
		case ModelData::Mode::Line:
			return GL_LINES;
		case ModelData::Mode::Triangle:
			return GL_TRIANGLES;
		default:
			assert(false);
			throw std::runtime_error("Undefined Draw Mode given");
		}
	}
}

namespace trm
{
namespace impl
{
	const GLuintType UNDEFINED_DRAW_MODE = std::numeric_limits<GLuintType>::max();

	struct State
		: boost::noncopyable
	{
		GLuintType buffVert;
		GLuintType buffIndx;
		GLuintType buffNorm;
		GLuintType drawMode;
		size_t indxCnt;

		State()
			: buffVert(InitBuffer())
			, buffIndx(InitBuffer())
			, buffNorm(InitBuffer())
			, drawMode(UNDEFINED_DRAW_MODE)
			, indxCnt(0)
		{}

		~State()
		{
			glDeleteBuffers(1, &buffNorm);
			glDeleteBuffers(1, &buffIndx);
			glDeleteBuffers(1, &buffVert);
		}
	};

} // namespace impl
} // namespace trm

ModelDrawer::ModelDrawer()
	: statePtr_(std::make_shared<impl::State>())
{
}

void 
ModelDrawer::Load(const ModelData & md)
{
	statePtr_->drawMode = ConvertDrawMode(md.type);
	statePtr_->indxCnt = md.indexes.size();

	if (md.points.empty())
		return;

	LoadBuffer(md.points, statePtr_->buffVert, GL_ARRAY_BUFFER);
	LoadBuffer(md.indexes, statePtr_->buffIndx, GL_ELEMENT_ARRAY_BUFFER);
#ifdef DRAWING_MODE_FULL
	LoadBuffer(md.normales, statePtr_->buffNorm, GL_ARRAY_BUFFER);
#endif // DRAWING_MODE_FULL
}

void 
ModelDrawer::Draw() const
{
	glBindBuffer(GL_ARRAY_BUFFER, statePtr_->buffVert);
	glEnableVertexAttribArray(DrawContext::VertexArray);
	glVertexAttribPointer(DrawContext::VertexArray, 3, GL_FLOAT, GL_FALSE, 0, 0);

#ifdef DRAWING_MODE_FULL
	glBindBuffer(GL_ARRAY_BUFFER, statePtr_->buffNorm);
	glEnableVertexAttribArray(DrawContext::NormalArray);
	glVertexAttribPointer(DrawContext::NormalArray, 3, GL_FLOAT, GL_FALSE, 0, 0);
#endif // DRAWING_MODE_FULL

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, statePtr_->buffIndx);
	glDrawElements(statePtr_->drawMode, statePtr_->indxCnt, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(DrawContext::VertexArray);
	glDisableVertexAttribArray(DrawContext::NormalArray);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}