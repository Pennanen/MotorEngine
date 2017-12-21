/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <graphics/GraphicsSystem.h>
#include <fstream>
#include <string>
#include "manager\FileManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb.h"
#include "stb\stb_image.h"

namespace engine
{	
	GLuint LoadShader(GLenum type, const char *shaderSrc)
	{
		GLuint shader;
		GLint compiled;
		// Create the shader object
		shader = glCreateShader(type);

		if (shader == 0)
			return 0;

		// Load the shader source
		glShaderSource(shader, 1, &shaderSrc, NULL);
		// Compile the shader
		glCompileShader(shader);
		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char) * infoLen);
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				free(infoLog);
			}
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
	//SHADER
	Shader::Shader(const char* vVertex, const char* vFragment)
	{
		GLuint vertexShader;
		GLuint fragmentShader;
		GLint linked;
		

		// Load the vertex/fragment shaders
		vertexShader = LoadShader(GL_VERTEX_SHADER, vVertex);
		fragmentShader = LoadShader(GL_FRAGMENT_SHADER, vFragment);
		this->programObject = glCreateProgram();
		glAttachShader(this->programObject, vertexShader);
		glAttachShader(this->programObject, fragmentShader);

		// Bind vPosition to attribute 0   
		glBindAttribLocation(this->programObject, 0, "vPosition");
		glBindAttribLocation(this->programObject, 1, "vTexCoord");

		// Link the program
		glLinkProgram(this->programObject);
		glGetProgramiv(this->programObject, GL_LINK_STATUS, &linked);
	}
	Shader::~Shader()
	{
	}

	void Shader::ComputeMatrixes(float _angle, float _x, float _y, float _sx, float _sy, float _sz)
	{
		float angle, x, y, sx, sy, sz;
		angle = _angle;
		x = _x;
		y = _y;
		sx = _sx;
		sy = _sy;
		sz = _sz;

		// Compute matrixes:
		// Translation
		this->m_model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
		// Rotationaround Oz with 45 degrees
		this->m_model = glm::rotate(this->m_model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		// Scale is 1.0 
		this->m_model = glm::scale(this->m_model, glm::vec3(sx, sy, 1.0f));
		this->m_view = glm::mat4(1.0f);
		this->m_projection = glm::ortho(0.0f, 400.0f, 300.0f, 0.0f);
	}

	void Shader::SetMVP()
	{
		this->mvp = this->m_projection * glm::inverse(this->m_view) * this->m_model;
		GLuint mvpLoc = glGetUniformLocation(this->programObject, "MVP");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	}

	void Shader::useShader()
	{
		glUseProgram(this->programObject);
	}

	GLuint Shader::getUniformLocation(const char* const uniformName)
	{
		return glGetUniformLocation(this->programObject, uniformName);
	}

	std::vector<char> GraphicsSystem::loadShaderFile(std::string path)
	{

		const char* pathC = path.c_str();

		FileManager mgr(pathC, FileManager::READ_ONLY);

		std::vector<char> fileData;
		fileData.resize(mgr.available() + 1);
		mgr.read(&fileData[0], fileData.size() - 1);
		fileData[fileData.size() - 1] = 0;
		return fileData;
	}

	Texture2D* GraphicsSystem::loadImageFile(std::string path)
	{
#if defined(_WIN32)
		const char* imagePath = path.c_str();
		int w;
		int h;
		int comp;
		GLubyte* image = stbi_load(imagePath, &w, &h, &comp, STBI_rgb);
		//m_textures.push_back(new Texture2D(w, h, comp, image));
		Texture2D* t = new Texture2D(w, h, comp, image);
		stbi_image_free(image);
		return t;
#endif
#if defined(ANDROID)
		const char* imagePath = path.c_str();
		int w;
		int h;
		int comp;

		FileManager mgr(imagePath, FileManager::READ_ONLY);
		size_t fileLen = mgr.available();
		stbi_uc* fileData = new unsigned char[fileLen];

		mgr.read(&fileData[0], mgr.available() - 1);

		unsigned char* image = stbi_load_from_memory(fileData, fileLen, &w, &h, &comp, 0);
		//m_textures.push_back(new Texture2D(w, h, comp, image));
		Texture2D* t = new Texture2D(w, h, comp, image);
		stbi_image_free(image);
		return t;
#endif
	}

	GraphicsSystem::GraphicsSystem()
	{
	}


	GraphicsSystem::~GraphicsSystem()
	{
	}
	//TEXTURE
	Texture2D::Texture2D(int width, int height, int bytesPerPixel, const GLubyte* pixels)
		: Object()
		, m_textureId(0)
	{
		assert(bytesPerPixel == 3 || bytesPerPixel == 4);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &m_textureId);
		glBindTexture(GL_TEXTURE_2D, m_textureId);

		// Load the texture
		glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel==3 ? GL_RGB : GL_RGBA, width, height, 0, bytesPerPixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Set the filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}


	Texture2D::~Texture2D()
	{
	}

	GLuint Texture2D::getTextureId()
	{
		return m_textureId;
	}

	void GraphicsSystem::drawTriangles(Shader* shader, Texture2D* texture, std::vector<GLfloat> vertices, std::vector<GLfloat> textureCoords, int numVertices)
	{

		//Pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());
		glEnableVertexAttribArray(0);
		//TexCoords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, textureCoords.data());
		glEnableVertexAttribArray(1);
		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
		//Set the sampler texture unit 0
		glUniform1i(shader->getUniformLocation("texture"), 0);

		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
}
