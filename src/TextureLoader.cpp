#include "TextureLoader.h"
#include "PngHandler.h"

#include <sstream>

using namespace trm;
using namespace utils;

#include <GLES2/gl2.h>

#include <boost/range/algorithm/transform.hpp>

TextureProperties
TextureLoader::Load(const std::string & path)
{
	utils::PngHandler pngHandler(path);
	const auto & props = pngHandler.Properties();
	ImgData imgData = pngHandler.GetData(ImgArea(0, 0, props.width, props.height));
	
	if (imgData.empty())
	{
		std::stringstream s;
		s << "Failed to initialize texture " << path;
		throw std::runtime_error(s.str());
	}

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, props.width, props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &imgData[0]);

	return TextureProperties(textureId, props);
}

void
TextureLoader::Delete(GLuintType id)
{
	glDeleteTextures(1, &id);
}