#define GL_GLEXT_PROTOTYPES

#include <SDL.h>
#include <SDL_opengles2.h>

#include <stdio.h>
#include <string>

SDL_Window * gWindow = NULL;
SDL_GLContext gContext = NULL;

GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

bool InitGL()
{
	bool success = true;

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
		printf("Unabe to compile vertex shader %d!\n", vertexShader);
		success = false;
	}
	else
	{
		printf("Compiled vertex shader %d!\n", vertexShader);

		glAttachShader(gProgramID, vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar * fragmentShaderSource[] =
		{
			"void main() {gl_FragColor = vec4(1.0, 1.0, 0.66, 1.0); }"
		};

		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		glCompileShader(fragmentShader);

		GLint vShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &vShaderCompiled);
		if (vShaderCompiled != GL_TRUE)
		{
			printf("Unabe to compile fragment shader %d!\n", fragmentShader);
			success = false;
		}
		else
		{
			printf("Compiled fragment shader %d!\n", fragmentShader);

			glAttachShader(gProgramID, fragmentShader);

			glLinkProgram(gProgramID);

			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", gProgramID);
				success = false;
			}
			else
			{
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
				if (gVertexPos2DLocation == -1)
				{
					printf("LVertexPos2D is not a valid glsl program variable!\n");
					success = false;
				}
				else
				{
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
			}
		}
	}

	return success;
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

bool Init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initalize! SDL Error %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_DisplayMode mode;

		SDL_GetDisplayMode(0, 0, &mode);
		int width = mode.w;
		int height = mode.h;

		gWindow = SDL_CreateWindow("Transport Mania", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			//640, 480,
			width, height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				if (SDL_GL_SetSwapInterval(-1) < 0)
				{
					if (SDL_GL_SetSwapInterval(1) < 0)
					{
						printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
					}
				}

				if (!InitGL())
				{
					printf("Unable to initialize OpenGL!");
					success = false;
				}
			}
		}
	}

	return success;
}


int main(int argc, char ** argv)
{
	(argc);
	(argv);

	if (!Init())
	{
		return 1;
	}

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

		Render();

		SDL_GL_SwapWindow(gWindow);
	}

	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	return 0;
}
