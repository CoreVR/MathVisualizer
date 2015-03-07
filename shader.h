#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include "graphicsmath.h"
#include <Kernel/OVR_Math.h>

class Shader
{
	public:
		Shader(std::string sourceDirectory);
		~Shader();
		Shader& operator=(const Shader& rhs);
		void Use();
		void SetUniforms(mat4 model, mat4 view, mat4 projection);
		void SetMVPMatrix(OVR::Matrix4f MVP );
		void SetViewMatrix(mat4 view);
		void SetProjMatrix(mat4 projection);
		void Delete();
	private:
		GLuint mvpLocation;
		void LoadFromText(std::string source); //each type has same name but different file type (.vert, .frag)
		std::string vertexShaderSource;
	    std::string fragmentShaderSource;
		GLuint shaderProgram;
		GLint modelLocation;
		GLint viewLocation;
		GLint projectionLocation;

};


#endif //SHADER_H