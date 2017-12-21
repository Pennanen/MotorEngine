/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _ENGINE_TEST_APPLICATION_H_
#define _ENGINE_TEST_APPLICATION_H_

#include <graphics/GraphicsApplication.h>
#include <core/Log.h>
#include <stdlib.h>
#include "Common/Include/esUtil.h"
#include <vector>
#include <graphics/GraphicsSystem.h>
#include <core/Ref.h>


namespace engine
{
	class Window;
	class GraphicsSystem;
	class GameObject
	{
	public:
		GameObject(GraphicsSystem* graphics, float _x, float _y, float _xspd,float _yspd);
		~GameObject();
		void Update(float deltaTime);
		Shader *s;
		std::vector<GLfloat> quad;
		std::vector<GLfloat> texCoords;
		float x;
		float y;
		float xspd;
		float yspd;
		float width = 20.0f;
		float height = 20.0f;
		//std::vector<float> velocity;
		float xV = 0.0f;
		float yV = 0.0f;
		std::vector<float> colliderBox;
		float xv, xo, yy, ya;
	protected:
	};
	class TestApplication :

		public GraphicsApplication
	{
	public:
		TestApplication(Window* window, GraphicsSystem* graphics);

		~TestApplication();

		/// Updates application. Returns true, if application is running.
		virtual bool update(float deltaTime);
		virtual void CreateObject(GraphicsSystem * graphics, float x, float y, float width, float height, float xspd, float yspd);
		/// Renders application.
		virtual void render(Window* window, GraphicsSystem* graphics);
		virtual void ResetGame(std::vector<GameObject> &gos,float deltaTime);
		virtual bool DoesCollide(GameObject a, GameObject b);
		std::vector< engine::Ref<Shader> > m_shaders;
		std::vector< engine::Ref<Texture2D> > m_textures;
		std::vector<GameObject> gos;
		float MAXBOUNCEANGLE = 1.30899694f;
		int p1score = 0;
		int p2score = 0;
		//GLuint programObject;
	private:
		float m_totalTime;
		float x;
		float y;
		std::vector<GLfloat[18]> quadVector;
	};


}

#endif

