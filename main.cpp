#include <GL/glew.h>
#if defined(_WIN32)
 #define GLFW_EXPOSE_NATIVE_WIN32
 #define GLFW_EXPOSE_NATIVE_WGL
 #define OVR_OS_WIN32
#elif defined(__APPLE__)
 #define GLFW_EXPOSE_NATIVE_COCOA
 #define GLFW_EXPOSE_NATIVE_NSGL
 #define OVR_OS_MAC
#elif defined(__linux__)
 #define GLFW_EXPOSE_NATIVE_X11
 #define GLFW_EXPOSE_NATIVE_GLX
 #define OVR_OS_LINUX
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OVR_CAPI_GL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "shader.h"
#include "player.h"
#include "camera.h"
#include "graphicsmath.h"

using namespace std;

float Player::winHeight=0.0f;
float Player::winWidth=0.0f;
bool Player::winResize=false;

void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
    	glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main()
{

	cout<<"f(x,y) = ";
	string equationText;
	getline(cin, equationText);
	puts("Generating plot...");

	//-----PARSING EQUATION-------//
	//variables in x, y
	float x,y,z;

	vector<float> numbers;
	vector<char> operators; // +,-,*,/,^
	vector<char> groupers; // (,),{,}, ,
	for(int i; i<equationText.size();i++)
	{
		
	}
	float maxX=0, maxY=0, maxZ=0, minZ=0;
	vector<float> xVals, yVals, zVals, points;
	for(int i=-100; i<101;i++)
	{
		xVals.push_back(float(i)/100.0f);
		if(xVals.back()>maxX || -xVals.back()>maxX)
		{
			maxX=xVals.back();
		}
		for(int j=-100;j<101;j++)
		{
			yVals.push_back(float(j)/100.0f);
			if(yVals.back()>maxY || -yVals.back()>maxY)
			{
				maxY=yVals.back();
			}
	
			zVals.push_back(xVals.back()*xVals.back());
			if(zVals.back()>maxZ)
			{
				maxZ=zVals.back();
			}
			else
			{
				if(zVals.back()<minZ)
				{
					minZ=zVals.back();
				}
			}

			points.push_back(xVals.back());
			points.push_back(zVals.back());
			points.push_back(yVals.back());
		}
	}
	int height=201;
	int width=201;
	vector<int> vertexIndices;
	int numberofStripsRequire=height-1;
	int numberofDegeneratesRequired=2*(numberofStripsRequire-1);
	int verticesPerStrip=2*width;
	for(int y=0;y<height-1;y++)
	{	
		//degenerate begins, repeate first vertex
		if(y>0)
		{
			vertexIndices.push_back(y*height);
		}

		//go through strip
		for(int x=0;x<width;x++)
		{
			
			vertexIndices.push_back((y*height)+x);
			vertexIndices.push_back(((y+1)*height)+x);
		}

		//Degenerate ends, repeat last vertex
		if(y<height-2)
		{
	
			vertexIndices.push_back(((y+1)*height)+(width-1));
		}

	}
	printf("max: %f, min: %f\n", maxZ, minZ);
	int pointCount=points.size();
	vector<float> colors;
	for(int i=0; i<pointCount; i+=3)
	{
		//if(maxX!=0) colors.push_back(points[i]*255/maxX);
		
		colors.push_back((-minZ+points[i+1])/(maxZ-minZ)); 
		if(colors.back()>1)
		{
			printf("%f\n", colors.back());
		}
		//if(maxY!=0) colors.push_back(points[i+2]/maxY);
		colors.push_back(0);
		colors.push_back(0);
	}
	char start;
	cout<<"Press enter to plot";
	getchar();
	cout<<"Plotting";

	//ovr
	ovr_Initialize();
	//ovrHmd hmd=ovrHmd_Create(0);
	float fov;
	//if(hmd==NULL)
	//{
	ovrHmd hmd=ovrHmd_CreateDebug(ovrHmd_DK2);
	//}else
	//{
	//	ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation|ovrTrackingCap_MagYawCorrection|ovrTrackingCap_Position, 0);
	//}


	if(!glfwInit())
	{
		fputs("ERROR: could not start GLFW3\n", stderr);
	}

	//FINDING MONITOR FOR EXTENDED MODE
	GLFWmonitor* hmdMonitor;

	int i, count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	for (i = 0;  i < count;  i++)
	{
	//#if defined(_WIN32)
	  //  if (strcmp(glfwGetWin32Monitor(monitors[i]), hmd->DisplayDeviceName) == 0)
	    //    hmdMonitor= monitors[i];
	//#elif defined(__APPLE__)
	  //  if (glfwGetCocoaMonitor(monitors[i]) == hmd->DisplayId)
	    //    hmdMonitor= monitors[i];
	//#elif defined(__linux__)
		int xpos, ypos;
		const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &xpos, &ypos);
		if (hmd->WindowsPos.x == xpos &&
	        hmd->WindowsPos.y == ypos &&
	        hmd->Resolution.w == mode->width &&
	        hmd->Resolution.h == mode->height)
	    {
	        hmdMonitor=monitors[i];
	    }
	}

	//const GLFWvidmode* mode=glfwGetVideoMode(hmdMonitor);
	//glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	//glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	//glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	//glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	
	GLFWwindow* window=glfwCreateWindow(640,480,"Function Visualizer", hmdMonitor, NULL);
	if(window==NULL)
	{
		fputs("ERROR: could not create GLFW window\n", stderr);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallBack);

	glewExperimental=GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);

	GLuint FBO=0;
	glGenFramebuffers(1 &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLuint renderTexture;
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2d(GL_TEXTURE_2D, 0, GL_REB, 1920,1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	

	//oculus stuff
	// Configure Stereo settings.
	Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left,
	hmd->DefaultEyeFov[0], 1.0f);
	Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right,
	hmd->DefaultEyeFov[1], 1.0f);
	Sizei renderTargetSize;
	renderTargetSize.w = recommenedTex0Size.w + recommenedTex1Size.w;
	renderTargetSize.h = max ( recommenedTex0Size.h, recommenedTex1Size.h );
	const int eyeRenderMultisample = 1;
	pRendertargetTexture = pRender->CreateTexture(
	Texture_RGBA | Texture_RenderTarget | eyeRenderMultisample,
	renderTargetSize.w, renderTargetSize.h, NULL);

	// The actual RT size may be different due to HW limits.
	renderTargetSize.w = pRendertargetTexture->GetWidth();
	renderTargetSize.h = pRendertargetTexture->GetHeight();


	Player player(window);
	Shader shader("shader");


	//GRAPH MODEL MATRIS
	vec3 worldPosition(0,0,-5);
	vec3 localScale(1,1,1);
	vec3 localRotation(0,0,0);
	mat4 scaleMat=scale(identity_mat4(), localScale);
	mat4 rotationMat;
	float quaternion[4];
	QuatFromAngle(quaternion, localRotation);
	QuatToMat4(rotationMat.m, quaternion);
	mat4 modelMat=translate(identity_mat4(), worldPosition)*rotationMat*scaleMat;

	shader.SetUniforms(modelMat, player.camera.viewMat, player.camera.projectionMat);

	glClearColor(1.0,1.0,1.0,1.0);
	glViewport(0, 0, 640, 480);


	//create graph axis vao
	float axis[]={1.0f,0.0f,0.0f,
				   -1.0f,0.0f,0.0f,
				   0.0f,1.0f,0.0f,
				   0.0f,-1.0f,0.0f,
				   0.0f,0.0f,1.0f,
				   0.0f,0.0f,-1.0f};

	float axisColor[]={0.0f, 0.3f, 0.0f,
						 0.0f, 0.3f, 0.0f,
						 0.0f, 0.3f, 0.0f,
						 0.0f, 0.3f, 0.0f,
						 0.0f, 0.3f, 0.0f,
     					 0.0f, 0.3f, 0.0f};
    GLuint gVBO[2]={0,0}; //points, colors
	GLuint gVAO=0;

	glGenBuffers(2, gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float), axis, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float), axisColor, GL_STATIC_DRAW);

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);					 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//plot vao
	GLuint VBO[2]={0,0}; //points, colors
	GLuint VAO=0;

	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, points.size()* sizeof(float), points.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(float), colors.data(), GL_STATIC_DRAW);

	GLuint IBO=0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size()*sizeof(int), vertexIndices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);



	while(!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glLineWidth(3);
		glBindVertexArray(gVAO);
		glDrawArrays(GL_LINES, 0, 6);

		glLineWidth(1);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLE_STRIP, (int)vertexIndices.size(), GL_UNSIGNED_INT, 0);



		player.Move();
		if(player.newProjMat)
		{
			shader.SetProjMatrix(player.camera.projectionMat);		
			player.newProjMat=false;	
		}
		if(player.camMoved)
		{

			QuatToMat4(player.camera.rotation.m, player.camera.quaternion);

			player.camera.position=player.camera.position+vec3(player.camera.forward)*-player.moveVector.v[2];
			player.camera.position=player.camera.position+vec3(player.camera.up)*player.moveVector.v[1];
			player.camera.position=player.camera.position+vec3(player.camera.right)*player.moveVector.v[0];
			player.moveVector=vec3(0.0,0.0,0.0);
			mat4 T=translate(identity_mat4(), vec3(player.camera.position));

			player.camera.viewMat=inverse(player.camera.rotation)*inverse(T);

			//print(player.camera.position);
			shader.SetViewMatrix(player.camera.viewMat);

			player.camMoved=false;
			player.camera.yaw=0.0f;
			player.camera.pitch=0.0f;
			player.camera.roll=0.0f;
		}
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	ovrHmd_Destroy(hmd);
	ovr_Shutdown();

	return 0;


}