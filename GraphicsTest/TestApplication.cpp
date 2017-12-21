/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// "THE BEER-WARE LICENSE" (Revision 42):
/// <mikko.romppainen@kamk.fi> wrote this file.  As long as you retain this notice you
/// can do whatever you want with this stuff. If we meet some day, and you think
/// this stuff is worth it, you can buy me a beer in return. Mikko Romppainen.
/// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <TestApplication.h>

#include <graphics/GraphicsSystem.h>
#include <graphics/Window.h>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <core/Log.h>
#include <fstream>
#include <string>
#include <math.h>
//#define STB_IMAGE_IMPLEMENTATION

//#include "stb\stb.h"
//#include "stb\stb_image.h"
#include "Win32Window.h"
#include <vector>
#include "manager\InputManager.h"

namespace engine
{
	TestApplication::TestApplication(Window* window, GraphicsSystem* graphics)
		: GraphicsApplication(window, graphics)
		, m_totalTime(0.0f)
	{
		m_textures.push_back(graphics->loadImageFile("p1.png"));
		m_textures.push_back(graphics->loadImageFile("p2.png"));
		m_textures.push_back(graphics->loadImageFile("ball.png"));
		x = 100.0f;
		y = 100.0f;

		/*int numObjs = 6;

		for (int obj = 0; obj < numObjs; obj++)
		{
			CreateObject(graphics);
		}*/
		float mutator1 = rand() % 200;
		float mutator2 = rand() % 200;
		float mutator3 = rand() % 200;
		float mutator4 = rand() % 200;
		std::cout << std::endl << "P1 Mutators: " << mutator1 << " and " << mutator2 << std::endl;
		std::cout << "P2 Mutators: " << mutator3 << " and " << mutator4 << std::endl;
		CreateObject(graphics, 50.0f, 150.0f, 3.0f, 34.0f,mutator1/400,mutator3/400);
		CreateObject(graphics, 350.0f, 150.0f, 3.0f, 34.0f,mutator2/400,mutator4/400);
		CreateObject(graphics, 200.0f, 150.0f, 4.0f, 4.0f,0.0f,0.0f);
		gos[2].xV = 4.0f;
		//gos[2].yV = 1.0f;
	}

	TestApplication::~TestApplication()
	{
	}

	void TestApplication::CreateObject(GraphicsSystem* graphics, float x, float y, float width, float height,float xspd,float yspd)
	{
		GameObject go(graphics, x, y,xspd,yspd);
		go.width = width;
		go.height = height;
		go.yspd = yspd;
		go.xspd = xspd;
		if(m_shaders.size() <= 0)
			m_shaders.push_back(go.s);
		gos.push_back(go);
	}

	bool TestApplication::update(float deltaTime)
	{
		m_totalTime += deltaTime;
		clearInput();

		/*
		if (!getKeyState(KeyCodes::KEY_W) && !getKeyState(KeyCodes::KEY_S) || gos[0].y <= 0.0f + (gos[0].height / 2) || gos[0].y >= 300.0f - (gos[0].height / 2))
		{
			if (gos[0].yV < -0.2f) { gos[0].yV += 0.2f; }
			else if (gos[0].yV > 0.2f) { gos[0].yV -= 0.2f; }
			else{ gos[0].yV = 0.0f; }

		if (gos[0].y >= 300.0f - (gos[0].height / 2)) { gos[0].yV *= -0.5f;; gos[0].y -= 1.0f; }
		if (gos[0].y <= 0.0f + (gos[0].height / 2)) { gos[0].yV *= -0.5f;; gos[0].y += 1.0f; };
		}
		if (getKeyState(KeyCodes::KEY_S) && gos[0].y >= 0.0f + (gos[0].height / 2) && gos[0].yV < 3.1f)
			gos[0].yV += (abs(gos[1].y - gos[2].y) / 400 + 0.3f);
		if (getKeyState(KeyCodes::KEY_W) && gos[0].y <= 300.0f - (gos[0].height / 2) && gos[0].yV > -3.1f)
			gos[0].yV -= (abs(gos[1].y - gos[2].y) / 400 + 0.3f);
		*/
		
		if (gos[2].y > gos[0].y && gos[0].y >= 0.0f + (gos[0].height / 2) && gos[0].yV < 3.0f && gos[2].xV < 0)
		{
			gos[0].yV += (abs(gos[0].y - gos[2].y) / 50 + 0.05f) + gos[0].xspd;
		}
		else if (gos[2].y < gos[0].y && gos[0].y <= 300.0f - (gos[0].height / 2) && gos[0].yV > -3.0f && gos[2].xV < 0)
		{
			gos[0].yV -= (abs(gos[0].y - gos[2].y) / 50 + 0.05f) + gos[0].xspd;
		}
		else if (gos[0].y >= 300.0f - (gos[0].height / 2)) { gos[0].yV *=-0.5f; gos[0].y -= 1.0f; }
		else if (gos[0].y <= 0.0f + (gos[0].height / 2)) { gos[0].yV *= -0.5f; gos[0].y += 1.0f; }
		


		if (gos[2].y > gos[1].y && gos[1].y >= 0.0f + (gos[1].height / 2) && gos[1].yV < 3.0f && gos[2].xV > 0)
		{
			gos[1].yV += (abs(gos[1].y-gos[2].y) / 50 + 0.04f)+gos[1].xspd;
		}
		else if (gos[2].y < gos[1].y && gos[1].y <= 300.0f - (gos[1].height / 2) && gos[1].yV > -3.0f && gos[2].xV > 0)
		{
			(gos[1].yV -= abs(gos[1].y - gos[2].y) / 50 + 0.04f) + gos[1].xspd;
		}
		else if (gos[1].y >= 300.0f - (gos[1].height / 2)) { gos[1].yV *= -0.5f; gos[1].y -= 1.0f; }
		else if (gos[1].y <= 0.0f + (gos[1].height / 2)) { gos[1].yV *= -0.5f; gos[1].y += 1.0f; }


		//apply speed


		//x = abs(sinf(2.0f*m_totalTime));
		for (unsigned int i = 0; i < gos.size(); i++)
		{
			gos[i].Update(deltaTime);
			gos[i].s->ComputeMatrixes(0,gos[i].x,gos[i].y, gos[i].width, gos[i].height,1.0f);
		}

		if (gos[2].y < 0.0f || gos[2].y > 300.0f)
			gos[2].yV *= -1;

		if (gos[2].x > 400.0f)
		{
			//Piste pelaajalle 1
			p1score++;
			ResetGame(gos,deltaTime);
			std::cout << "SCORE:" << std::endl;
			std::cout << "Player 1: " << p1score << std::endl;
			std::cout << "Player 2: " << p2score << std::endl;
		}
		if (gos[2].x < 0.0f)
		{
			//Piste pelaajalle 2
			p2score++;
			ResetGame(gos,deltaTime);
			std::cout << "SCORE:" << std::endl;
			std::cout << "Player 1: " << p1score << std::endl;
			std::cout << "Player 2: " << p2score << std::endl;
		}

		if (DoesCollide(gos[1], gos[2]))
		{
			float relativeIntersectY = gos[1].y + (gos[1].height / 2.0f) - (gos[2].y-gos[1].y);
			float normalizedRelativeIntersectionY = (relativeIntersectY / (gos[1].height / 2.0f));
			float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
			gos[2].xV = -2.0f-abs(gos[1].yV)/1.5; // 1.0f*cos(bounceAngle);
			gos[2].yV = 1.0f*sin(bounceAngle);
		}
		if (DoesCollide(gos[0], gos[2]))
		{
			float relativeIntersectY = gos[0].y + (gos[0].height / 2.0f) - (gos[2].y - gos[0].y);
			float normalizedRelativeIntersectionY = (relativeIntersectY / (gos[0].height / 2.0f));
			float bounceAngle = normalizedRelativeIntersectionY * MAXBOUNCEANGLE;
			gos[2].xV = 2.0f+abs(gos[0].yV)/1.5; //1.0f*cos(bounceAngle);
			gos[2].yV = 1.0f*sin(bounceAngle);
		}


		return true;
	}

	void TestApplication::render(Window* window, GraphicsSystem* graphics)
	{
		(void)window;	

		glViewport(0, 0, window->getWidth(), window->getHeight());
		glClear(GL_COLOR_BUFFER_BIT);

		for (unsigned int i = 0; i < gos.size(); i++)
		{
			gos[i].s->useShader();
			gos[i].s->SetMVP();

				graphics->drawTriangles(gos[i].s, m_textures[i], gos[i].quad, gos[i].texCoords, 6);

		}

		graphics->swapBuffers();
	}

	void TestApplication::ResetGame(std::vector<GameObject> &gos,float deltaTime)
	{
		/*
		CreateObject(graphics, 50.0f, 50.0f, 4.0f, 48.0f);
		CreateObject(graphics, 350.0f, 50.0f, 4.0f, 48.0f);
		CreateObject(graphics, 200.0f, 50.0f, 5.0f, 5.0f);
		gos[2].xV = 1.0f;
		*/

		gos[0].y = 150.0f;
		gos[0].yV = 0.0f;
		gos[1].y = 150.0f;
		gos[1].yV = 0.0f;
		gos[2].x = 250.0f;
		gos[2].y = 150.0f;
		gos[2].xV = 2.0f;
		gos[2].yV = 0.0f;





		
		for (unsigned int i = 0; i < gos.size(); i++)
		{
			gos[i].Update(deltaTime);
			gos[i].s->ComputeMatrixes(0, gos[i].x, gos[i].y, gos[i].width, gos[i].height, 1.0f);
		}
		return;
	}

	GameObject::GameObject(GraphicsSystem* graphics, float _x, float _y,float _xspd,float _yspd)
	{
		float _size = 1.0f;
		float _dx = -0.5f;
		float _dy = -0.5f;
		float _depth = 0.0f;
		x = _x;
		y = _y;
		s = new Shader(&graphics->loadShaderFile("vShaderStr.txt")[0], &graphics->loadShaderFile("fShaderStr.txt")[0]); //graphics->loadShaderFile("vShaderStr.txt", "fShaderStr.txt");
		//width = 20.0f;
		//height = 20.0f;
		quad = {
			_dx + 0.0f, _dy + _size, _depth, //vertex 0 vasen ala
			_dx + 0.0f, _dy + 0.0f, _depth, //vertex 1 vasen ylä
			_dx + _size, _dy + 0.0f, _depth, //vertex 2 oikea ylä
			_dy + _size, _dy + _size, _depth, //vertex 3 oikea ala
			_dy + _size, _dy + 0.0f, _depth, //vertex 4 oikea ylä
			_dy + 0.0f, _dy + _size, _depth //vertex 5 oikea ala
		};

		xv = quad[0] + x;
		xo = quad[6] + x;
		yy = quad[4] + y;
		ya = quad[1] + y;
		colliderBox = { xv, yy, xo, ya };

		texCoords = {
			0,0,
			0,1,
			1,1,
			1,0,
			1,1,
			0,0
		};
	}
	GameObject::~GameObject()
	{
	}

	void GameObject::Update(float deltaTime)
	{
		this->xv = this->quad[0] + this->x;
		this->xo = this->quad[6] + this->x;
		this->yy = this->quad[4] + this->y;
		this->ya = this->quad[1] + this->y;
		this->colliderBox = { this->xv, this->yy, this->xo, this->ya };
		this->y += this->yV*deltaTime*100;
		this->x += this->xV*deltaTime*100;
	}

	bool TestApplication::DoesCollide(GameObject a, GameObject b)
	{
		return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
			(abs(a.y - b.y) * 2 < (a.height + b.height));
	}
}