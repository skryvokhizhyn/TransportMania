#define GL_GLEXT_PROTOTYPES

#include "Game.h"
#include "Logger.h"

#include <SDL_opengles2.h>

#include <boost/format.hpp>
#include <stdexcept>

using namespace trm;

namespace trm
{
	// exported for each platform
	void GetScreenParameters(int & width, int & height, uint32_t & flags);
}

namespace
{
	GLuint gProgramID = 0;
	GLint gVertexPos2DLocation = -1;
	GLuint gVBO = 0;
	GLuint gIBO = 0;

	void InitGLObjects()
	{
		gProgramID = glCreateProgram();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar * vertexShaderSource[] =
		{
			"attribute vec2 LVertexPos2D; void main() {gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1);}"
		};

		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			throw std::runtime_error((boost::format("Unabe to compile vertex shader %d!") % vertexShader).str());
		}

		utils::Logger().Debug() << (boost::format("Compiled vertex shader %d!") % vertexShader).str();

		glAttachShader(gProgramID, vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar * fragmentShaderSource[] =
		{
			"void main() {gl_FragColor = vec4(1.0, 1.0, 0.66, 1.0); }"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		glCompileShader(fragmentShader);

		vShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			throw std::runtime_error((boost::format("Unabe to compile fragment shader %d!") % fragmentShader).str());
		}

		utils::Logger().Debug() << (boost::format("Compiled fragment shader %d!") % fragmentShader).str();

		glAttachShader(gProgramID, fragmentShader);

		glLinkProgram(gProgramID);

		GLint programSuccess = GL_TRUE;
		glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE)
		{
			throw std::runtime_error((boost::format("Error linking program %d!") % gProgramID).str());
		}

		gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
		if (gVertexPos2DLocation == -1)
		{
			throw std::runtime_error("LVertexPos2D is not a valid glsl program variable!");
		}

		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLfloat vertexData[] =
		{
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f
		};

		GLuint indexData[] = { 0, 1, 2, 3 };

		glGenBuffers(1, &gVBO);
		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

		glGenBuffers(1, &gIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//	if (gRendererQuad)
		{
			glUseProgram(gProgramID);

			glEnableVertexAttribArray(gVertexPos2DLocation);

			glBindBuffer(GL_ARRAY_BUFFER, gVBO);
			glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

			glDisableVertexAttribArray(gVertexPos2DLocation);

			glUseProgram(0);
		}
	}

} // namespace

Game::SDL_RAII::SDL_RAII()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error((boost::format("SDL could not initalize! SDL Error %s") % SDL_GetError()).str());
	}
}

Game::SDL_RAII::~SDL_RAII()
{
	SDL_Quit();
}

Game::Game()
{
	InitSDL();
	InitGL();
	//InitGLObjects();

	app_.InitApplication(640, 480);
	app_.InitView();
}

Game::~Game()
{
	app_.ReleaseView();
	app_.QuitApplication();
}

void
Game::InitSDL()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int width = 0;
	int height = 0;
	uint32_t flags = 0;

	GetScreenParameters(width, height, flags);

	windowPtr_.reset(
		SDL_CreateWindow("Transport Mania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height, flags),
		[](SDL_Window * p) { SDL_DestroyWindow(p); }
	);

	if (!windowPtr_)
	{
		throw std::runtime_error((boost::format("Window could not be created! SDL Error: %s") % SDL_GetError()).str());
	}	
}

void
Game::InitGL()
{
	contextPtr_.reset(
		SDL_GL_CreateContext(windowPtr_.get()),
		[](SDL_GLContext p) { SDL_GL_DeleteContext(p); }
	);

	if (!contextPtr_)
	{
		throw std::runtime_error((boost::format("OpenGL context could not be created! SDL Error: %s") % SDL_GetError()).str());
	}

	if (SDL_GL_SetSwapInterval(-1) < 0)
	{
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			utils::Logger().Warning() << (boost::format("Warning: Unable to set VSync! SDL Error: %s") % SDL_GetError()).str();
		}
	}
}

void
Game::Run()
{
	Uint8 done = 0;
	SDL_Event event;
	
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN)
			{
				done = 1;
			}
		}
	
		//Render();
		app_.RenderScene();
	
		SDL_GL_SwapWindow(windowPtr_.get());
	}
}
