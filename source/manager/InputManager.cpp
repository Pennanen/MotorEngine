#include <manager/InputManager.h>
#include <assert.h>
#include <core/Log.h>
#if defined(ANDROID)
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <string.h>

#endif
#include <Windows.h>


namespace engine
{



	// anonymous namespace for internal functions
	namespace
	{
		bool clicks[3];
		bool prevClicks[3];
		int mouseXValue = 0;
		int mouseYValue = 0;
		int mouseWheelDelta = 0;
		bool keys[0xff];
		bool prevKeys[0xff];
		std::vector< Touch > touches;
	}

	void clearInput()
	{
		mouseWheelDelta = 0;
		memcpy(&prevClicks[0], &clicks[0], sizeof(clicks));

		memcpy(&prevKeys[0], &keys[0], sizeof(keys));
		for (int i = 0; i < 0xff; ++i)
		{
			keys[i] = (GetAsyncKeyState(i) & 0x8000) ? 1 : 0;
		}
	}


	void mouseWheel(int mouseWheel)
	{
		mouseWheelDelta += mouseWheel;
	}

	void mouseState(bool leftClicked, bool rightClicked, bool middleClicked, int mouseX, int mouseY)
	{
		clicks[0] = leftClicked;
		clicks[1] = rightClicked;
		clicks[2] = middleClicked;
		mouseXValue = mouseX;
		mouseYValue = mouseY;
	}

	int getMouseButtonState(MouseButtons button)
	{
		return clicks[button];
	}

	int isMouseButtonReleased(MouseButtons button)
	{
		// Prev pressed && now not pressed
		return prevClicks[button] && !clicks[button];
	}

	int isMouseButtonPressed(MouseButtons button)
	{
		// Now pressed && prev not pressed
		return clicks[button] && !prevClicks[button];
	}

	int getMouseAxisX()
	{
		return mouseXValue;
	}

	int getMouseAxisY()
	{
		return mouseYValue;
	}

	int getMouseWheelDelta()
	{
		return mouseWheelDelta;
	}

	int getKeyState(KeyCodes keyCode)
	{
		return keys[keyCode];
	}

	int isKeyPressed(KeyCodes keyCode)
	{
		// Now pressed && prev not pressed
		return keys[keyCode] && !prevKeys[keyCode];
	}

	int isKeyReleased(KeyCodes keyCode)
	{
		// Prev pressed && now not pressed
		return prevKeys[keyCode] && !keys[keyCode];
	}


	const std::vector<Touch>& getActiveTouches()
	{
		return touches;
	}


}