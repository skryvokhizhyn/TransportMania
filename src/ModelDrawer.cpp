#include "ModelDrawer.h"
#include "Point3d.h"
#include "ModelData.h"
#include "Logger.h"
#include "DrawContext.h"
#include "GlobalDefines.h"
#include "Types.h"
#include "TextureManagerProxy.h"
#include "ShaderAttribute.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <limits>

using namespace trm;

// we must preserve the size of Point3d as raw 3 * AxisType
// buffer creation relies on it!
static_assert(sizeof(Point3d) == 3 * sizeof(AxisType), "Expecting Point3d to be equal by size to 3 * size of AxisType");
static_assert(sizeof(Point2d) == 2 * sizeof(AxisType), "Expecting Point2d to be equal by size to 2 * size of AxisType");

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
		if (vec.empty())
			return;

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
		case ModelData::Mode::LineLoop:
			return GL_LINE_LOOP;
		case ModelData::Mode::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		case ModelData::Mode::TriangleFan:
			return GL_TRIANGLE_FAN;
		default:
			assert(false);
			throw std::runtime_error("Undefined Draw Mode given");
		}
	}

	GLuintType GetTextureInternalId(TextureId id)
	{
		const auto & props = TextureManagerProxy()->Get(id);
		return props.id;
	}
}

namespace trm
{
namespace impl
{
	const GLuintType UNDEFINED_DRAW_MODE = std::numeric_limits<GLuintType>::max();
	const GLuintType UNDEFINED_TEXTURE_ID = std::numeric_limits<GLuintType>::max();

	struct State
		: boost::noncopyable
	{
		GLuintType buffVert;
		GLuintType buffIndx;
		GLuintType buffNorm;
		GLuintType buffText;
		GLuintType drawMode;
		GLuintType textureId;
		size_t indxCnt;

		State()
			: buffVert(InitBuffer())
			, buffIndx(InitBuffer())
			, buffNorm(InitBuffer())
			, buffText(InitBuffer())
			, drawMode(UNDEFINED_DRAW_MODE)
			, textureId(UNDEFINED_TEXTURE_ID)
			, indxCnt(0)
		{}

		~State()
		{
			glDeleteBuffers(1, &buffText);
			glDeleteBuffers(1, &buffNorm);
			glDeleteBuffers(1, &buffIndx);
			glDeleteBuffers(1, &buffVert);
		}
	};

} // namespace impl
} // namespace trm

ModelDrawer::ModelDrawer()
	: statePtr_(std::make_shared<impl::State>())
{}

void 
ModelDrawer::Load(const ModelData & md)
{
	statePtr_->drawMode = ConvertDrawMode(md.type);
	statePtr_->textureId = GetTextureInternalId(md.textureId);
	statePtr_->indxCnt = md.indexes.size();

	if (md.points.empty())
		return;

	LoadBuffer(md.points, statePtr_->buffVert, GL_ARRAY_BUFFER);
	LoadBuffer(md.indexes, statePtr_->buffIndx, GL_ELEMENT_ARRAY_BUFFER);
#ifdef DRAWING_MODE_FULL
	LoadBuffer(md.normales, statePtr_->buffNorm, GL_ARRAY_BUFFER);
	LoadBuffer(md.textures, statePtr_->buffText, GL_ARRAY_BUFFER);
#endif // DRAWING_MODE_FULL
}

void 
ModelDrawer::Draw() const
{
	glBindBuffer(GL_ARRAY_BUFFER, statePtr_->buffVert);
	glEnableVertexAttribArray(ShaderAttribute::VertexArray);
	glVertexAttribPointer(ShaderAttribute::VertexArray, 3, GL_FLOAT, GL_FALSE, 0, 0);

#ifdef DRAWING_MODE_FULL
	glBindBuffer(GL_ARRAY_BUFFER, statePtr_->buffNorm);
	glEnableVertexAttribArray(ShaderAttribute::NormalArray);
	glVertexAttribPointer(ShaderAttribute::NormalArray, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, statePtr_->textureId);

	glBindBuffer(GL_ARRAY_BUFFER, statePtr_->buffText);
	glEnableVertexAttribArray(ShaderAttribute::TextureArray);
	glVertexAttribPointer(ShaderAttribute::TextureArray, 2, GL_FLOAT, GL_FALSE, 0, 0);
#endif // DRAWING_MODE_FULL

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, statePtr_->buffIndx);
	glDrawElements(statePtr_->drawMode, statePtr_->indxCnt, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(ShaderAttribute::VertexArray);
	glDisableVertexAttribArray(ShaderAttribute::NormalArray);
	glDisableVertexAttribArray(ShaderAttribute::TextureArray);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}