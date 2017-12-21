/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _ENGINE_GRAPHICSSYSTEM_H_
#define _ENGINE_GRAPHICSSYSTEM_H_

#include <core/Object.h>
#include <stdint.h>
#include <string>
#include <OGL\OGL.h>
#include <vector>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
#include <core/Ref.h>

namespace engine
{
	class Shader :
		public Object
	{
	public:
		Shader(const char* vShader, const char* fShader);
		~Shader();
		GLuint programObject;
		glm::mat4 mvp;
		virtual void useShader();
		virtual GLuint getUniformLocation(const char* const uniformName);
		void ComputeMatrixes(float angle, float x = 100.0f, float y = 100.0f, float sx = 100.0f, float sy = 100.0f, float sz = 100.0f);
		void SetMVP();

	protected:
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_model;
	};
	class Texture2D :
		public Object
	{
	public:
		Texture2D(int width, int height, int bytesPerPixel, const GLubyte* pixels);
		~Texture2D();
		GLuint m_textureId;
		virtual GLuint getTextureId();

	};
	class GraphicsSystem :
		public Object
	{
	public:
		std::vector<char> loadShaderFile(std::string path);
		Texture2D * loadImageFile(std::string path);
		GraphicsSystem();
		~GraphicsSystem();

		// Clears the screen using given color
		virtual void clearScreen(float red, float green, float blue, bool setViewport) = 0;

		// Method for swapping backbuffer to screen.
		virtual void swapBuffers() = 0;

		virtual void drawTriangles(Shader* shader, Texture2D* texture, std::vector<GLfloat> vertices, std::vector<GLfloat> textureCoords, int numVertices);
		//virtual Shader* GraphicsSystem::loadShaderFile(std::string vShadertxt, std::string fShadertxt);
	};

}

#endif

