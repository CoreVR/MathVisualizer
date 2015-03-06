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
#include "Kernel/OVR_Math.h"
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
using namespace OVR;

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
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLuint renderBuffer=0;
	glGenRenderbuffers(1, &renderBuffer);


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
	GLuint renderTexture;
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderTargetSize.w,renderTargetSize.h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture,0);

	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);


	ovrFovPort eyeFov[2]={hmd->DefaultEyeFov[0], hmd->DefaultEyeFov[1]};

	ovrRecti eyeRenderViewport[2];
	eyeRenderViewport[0].Pos=Vector2i(0,0);
	eyeRenderViewport[0].Size=Sizei(renderTargetSize.w/2, renderTargetSize.h);
	eyeRenderViewport[1].Pos=Vector2i(renderTargetSize.w+1/2,0);
	eyeRenderViewport[1].Size=eyeRenderViewport[0].Size;

	ovrGLTexture eyeTexture[2];
	eyeTexture[0].OGL.Header.API=ovrRenderAPI_OpenGL;
	eyeTexture[0].OGL.Header.TextureSize=renderTargetSize;
	eyeTexture[0].OGL.Header.RenderViewport=eyeRenderViewport[0];
	eyeTexture[0].OGL.TexId=renderTexture;

	eyeTexture[1]=eyeTexture[0];
	eyeTexture[1].OGL.Header.RenderViewport=eyeRenderViewport[1];

	//glfw stuff to set up ovr renderer
	int gwidth, gheight;
    union ovrGLConfig config;
    glfwGetFramebufferSize(window, &gwidth, &gheight);
    config.OGL.Header.BackBufferSize.w = gwidth;
    config.OGL.Header.BackBufferSize.h = gheight;
    config.OGL.Header.API=ovrRenderAPI_OpenGL;
    config.OGL.Header.TextureSize=Sizei(hmd->Resolution.w, hmd->Resolution.h);
    config.OGL.Header.Multisample=1;
	#if defined(_WIN32)
		if(!(hmd->HmdCaps & ovrHmdCap_ExtendedDesktop))
			ovrHmd_AttachToWindow(hmd, glfwGetWin32Window(window), NULL, NULL);
    	config.OGL.Window = glfwGetWin32Window(window);
   		config.OGL.DC=NULL;
	#elif defined(__APPLE__)
	#elif defined(__linux__)
    	config.OGL.Disp = glfwGetX11Display();
    	config.OGL.Win=glfwGetX11Window(window);
	#endif

    ovrEyeRenderDesc eyeRenderDesc[2];
    ovrHmd_ConfigureRendering(hmd, &config.Config, ovrDistortionCap_Chromatic|ovrDistortionCap_Vignette|ovrDistortionCap_TimeWarp|ovrDistortionCap_Overdrive, eyeFov, eyeRenderDesc);

    ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence|ovrHmdCap_DynamicPrediction);

    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation|ovrTrackingCap_MagYawCorrection|ovrTrackingCap_Postition,0);




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