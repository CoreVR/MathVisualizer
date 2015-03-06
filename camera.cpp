#include "camera.h"
#include <GL/gl.h>
#include "graphicsmath.h"

Camera::Camera()
{
	aspect=640.0f/480.0f;
	pitch=0.0f;
	
	SetSpeed(5.0f,10000.0f,10000.0f,10000.0f);
	SetFrustum(0.1f, 100.0f, 67.0f);

	forward=vec4(0.0f,0.0f,-1.0f,0.0f);
	right=vec4(1.0f,0.0f,0.0f,0.0f);
	up=vec4(0.0f,1.0f,0.0f,0.0f);

	position=vec3(0.0f,0.0f,5.0f);

	CreateProjectionMat();
	CreateViewMat();

}
Camera::Camera(int width, int height)
{
	aspect=(float)width/(float)height;
	pitch=0.0f;
	yaw=0.0f;
	roll=0.0f;

	SetSpeed(100.0f,10000.0f,10000.0f,10000.0f);
	SetFrustum(0.1f, 100.0f, 67.0f);

	forward=vec4(0.0f,0.0f,-1.0f,0.0f);
	right=vec4(1.0f,0.0f,0.0f,0.0f);
	up=vec4(0.0f,1.0f,0.0f,0.0f);

	position=vec3(0.0f,0.0f,0.0f);

	CreateProjectionMat();
	CreateViewMat();
}

void Camera::SetSpeed(float speed)
{
	camSpeed=speed;
}

Camera::~Camera()
{

}

void Camera::SetSpeed(float speed, float pitch, float yaw, float roll)
{
	camSpeed=speed;
	pitchSpeed=pitch;
	yawSpeed=yaw;
	rollSpeed=roll;
}

void Camera::SetAspect(float width, float height)
{
	aspect=width/height;
}

void Camera::SetFrustum(float near, float far, float fovy)
{
	nearPlane=near;
	farPlane=far;
	fieldofViewY=fovy;
}

void Camera::CreateProjectionMat()
{
	projectionMat = perspective(fieldofViewY, aspect, nearPlane, farPlane);
}

void Camera::CreateViewMat()
{
	translation=translate(identity_mat4(), vec3(-position.v[0], -position.v[1], -position.v[2]));

	
	CreateVersor(quaternion, -pitch, 0.0f, 1.0f, 0.0f);
	QuatToMat4(rotation.m, quaternion);
	viewMat=rotation*translation;
}