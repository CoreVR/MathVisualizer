#ifndef PLAYER_H
#define PLAYER_H
#include "camera.h"
#include <GLFW/glfw3.h>

class Player 
{
	public:
		Player(GLFWwindow* win);
		~Player();
		Camera camera;
		bool camMoved;
		vec3 moveVector;
		void Move();
		static float winWidth;
		static float winHeight;
		static bool winResize;
		bool newProjMat;
		static void WindowSizeCallback(GLFWwindow* win, int width, int height);

	private:
		GLFWwindow* window;
		float currentTime;
		float previousTime;
		float elapsedTime;


};

#endif