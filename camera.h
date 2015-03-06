#ifndef CAMERA_H
#define CAMERA_H
#include "graphicsmath.h"

class Camera
{
	public:
		Camera();
		Camera(int width, int height);
		~Camera();
		void SetFrustum(float near, float far, float fovy);
		void CreateProjectionMat();
		void CreateViewMat();
		void SetAspect(float width, float height);
		void SetSpeed(float speed);
		void SetSpeed(float speed, float pitch, float yaw, float roll);
		mat4 projectionMat;
		mat4 viewMat;
		float camSpeed;
		float pitchSpeed; //y axis
		float yawSpeed; //z axis
		float rollSpeed; //x axis
		float pitch;
		float yaw;
		float roll;
		vec4 forward;
		vec4 right;
		vec4 up;
		mat4 translation;
		mat4 rotation;
		float quaternion[4];
		vec3 position;

	private:
		float nearPlane;
		float farPlane;
		float fieldofViewY;
		float aspect;
	


};

#endif