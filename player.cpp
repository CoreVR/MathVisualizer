#include "player.h"
#include "camera.h"
#include <stdio.h>

Player::Player(GLFWwindow* win)
{
	int width, height;
	glfwGetWindowSize(win, &width, &height);
	window=win;
	camera=Camera(width,height);
	camMoved=false;
	moveVector=vec3(0.0f,0.0f,0.0f);
	newProjMat=false;
}


Player::~Player()
{

}

void Player::WindowSizeCallback(GLFWwindow* win, int width, int height)
{
	winWidth=width;
	winHeight=height;
	winResize=true;
}

void Player::Move()
{
	if(winResize)
	{
		newProjMat=true;
		//fputs("resize\n", stderr);
		winResize=false;
		camera.SetAspect(winWidth, winHeight);
		camera.CreateProjectionMat();
		glViewport(0, 0, winWidth, winHeight);

	}
	previousTime=glfwGetTime();
	//glfwSwapBuffers(window);
	glfwPollEvents();
	currentTime=glfwGetTime();
	elapsedTime=currentTime-previousTime;
	if (glfwGetKey (window, GLFW_KEY_A)) {
		moveVector.v[0] -= camera.camSpeed * elapsedTime;
		camMoved = true;
	}
	if (glfwGetKey (window, GLFW_KEY_D)) {
		moveVector.v[0] += camera.camSpeed * elapsedTime;			
		camMoved = true;
	}
	if (glfwGetKey (window, GLFW_KEY_Q)) {
		moveVector.v[1] += camera.camSpeed * elapsedTime;
		camMoved = true;
	}
	if (glfwGetKey (window, GLFW_KEY_E)) {
		moveVector.v[1] -= camera.camSpeed * elapsedTime;
		camMoved = true;
	}
	if (glfwGetKey (window, GLFW_KEY_W)) {
		moveVector.v[2] -= camera.camSpeed * elapsedTime;
		camMoved = true;
	}
	if (glfwGetKey (window, GLFW_KEY_S)) {
		moveVector.v[2] += camera.camSpeed * elapsedTime;
		camMoved = true;
	}
	if (glfwGetKey (window, GLFW_KEY_LEFT)) {
		camera.yaw += camera.yawSpeed * elapsedTime;
		camMoved = true;
	// create a camera.quaternion representing change in heading (the yaw)
		float q_yaw[16];
		CreateVersor (q_yaw, camera.yaw, camera.up.v[0], camera.up.v[1], camera.up.v[2]);
		// add yaw rotation to the camera's current orientation
		MultQuatQuat (camera.quaternion, q_yaw, camera.quaternion);
		
		// recalc axes to suit new orientation
		QuatToMat4 (camera.rotation.m, camera.quaternion);
		camera.forward = camera.rotation * vec4 (0.0, 0.0, -1.0, 0.0);
		camera.right = camera.rotation * vec4 (1.0, 0.0, 0.0, 0.0);			
		camera.up = camera.rotation * vec4 (0.0, 1.0, 0.0, 0.0);
		
	}
		
	if (glfwGetKey (window, GLFW_KEY_RIGHT)) {
		camera.yaw -= camera.yawSpeed * elapsedTime;
		camMoved = true;
		float q_yaw[16];
		CreateVersor (q_yaw, camera.yaw, camera.up.v[0], camera.up.v[1], camera.up.v[2]);
		MultQuatQuat (camera.quaternion, q_yaw, camera.quaternion);
		
		// recalc axes to suit new orientation
		QuatToMat4 (camera.rotation.m, camera.quaternion);
		camera.forward = camera.rotation * vec4 (0.0, 0.0, -1.0, 0.0);
		camera.right = camera.rotation * vec4 (1.0, 0.0, 0.0, 0.0);
		camera.up = camera.rotation * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (window, GLFW_KEY_UP)) {
		camera.pitch += camera.pitchSpeed * elapsedTime;
		camMoved = true;
		float q_pitch[16];
		CreateVersor (q_pitch, camera.pitch, camera.right.v[0], camera.right.v[1], camera.right.v[2]);
		MultQuatQuat (camera.quaternion, q_pitch, camera.quaternion);
		
		// recalc axes to suit new orientation
		QuatToMat4 (camera.rotation.m, camera.quaternion);
		camera.forward = camera.rotation * vec4 (0.0, 0.0, -1.0, 0.0);
		camera.right = camera.rotation * vec4 (1.0, 0.0, 0.0, 0.0);
		camera.up = camera.rotation * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (window, GLFW_KEY_DOWN)) {
		camera.pitch -= camera.pitchSpeed * elapsedTime;
		camMoved = true;
		float q_pitch[16];
		CreateVersor (q_pitch, camera.pitch, camera.right.v[0], camera.right.v[1], camera.right.v[2]);
		MultQuatQuat (camera.quaternion, q_pitch, camera.quaternion);
			
		// recalc axes to suit new orientation
		QuatToMat4 (camera.rotation.m, camera.quaternion);
		camera.forward = camera.rotation * vec4 (0.0, 0.0, -1.0, 0.0);
		camera.right = camera.rotation * vec4 (1.0, 0.0, 0.0, 0.0);
		camera.up = camera.rotation * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (window, GLFW_KEY_Z)) {
		camera.roll -= camera.rollSpeed * elapsedTime;
		camMoved = true;
		float q_roll[16];
		CreateVersor (q_roll, camera.roll, camera.forward.v[0], camera.forward.v[1], camera.forward.v[2]);
		MultQuatQuat (camera.quaternion, q_roll, camera.quaternion);
			
		// recalc axes to suit new orientation
		QuatToMat4 (camera.rotation.m, camera.quaternion);	
		camera.forward = camera.rotation * vec4 (0.0, 0.0, -1.0, 0.0);
		camera.right = camera.rotation * vec4 (1.0, 0.0, 0.0, 0.0);
		camera.up = camera.rotation * vec4 (0.0, 1.0, 0.0, 0.0);
	}
	if (glfwGetKey (window, GLFW_KEY_C)) {
		camera.roll += camera.rollSpeed * elapsedTime;
		camMoved = true;
		float q_roll[16];
		CreateVersor (q_roll, camera.roll, camera.forward.v[0], camera.forward.v[1], camera.forward.v[2]);
		MultQuatQuat (camera.quaternion, q_roll, camera.quaternion);
			
		// recalc axes to suit new orientation
		QuatToMat4 (camera.rotation.m, camera.quaternion);
		camera.forward = camera.rotation * vec4 (0.0, 0.0, -1.0, 0.0);
		camera.right = camera.rotation * vec4 (1.0, 0.0, 0.0, 0.0);
		camera.up = camera.rotation * vec4 (0.0, 1.0, 0.0, 0.0);
	}
}