#include "shader.h"
#include <cstring>
#include <stdio.h>
#include <cstdlib>

Shader::Shader(std::string sourceDirectory)
{

	Shader::LoadFromText(sourceDirectory);

	const char* vertexShaderSourceString=vertexShaderSource.c_str();
	const char* fragmentShaderSourceString=fragmentShaderSource.c_str();
	const GLint vertexLength=vertexShaderSource.length();
	const GLint fragmentLength=fragmentShaderSource.length();

	GLint compileStatus=0;
	//printf("vertex shader: %s\n\n\n\n", vertexShaderSourceString);
	//printf("fragment shader: %s\n\n\n", fragmentShaderSourceString);
	GLuint vertexShader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceString, &vertexLength);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus==GL_FALSE)
	{
		fprintf(stderr, "ERROR: Could not compile vertex shader %s\n", sourceDirectory.c_str());
		GLint logSize=0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		char* compileLog=(char*)malloc(logSize*sizeof(char));
		glGetShaderInfoLog(vertexShader, logSize, &logSize, compileLog);
		fputs(compileLog, stderr);
		free(compileLog);
	}

	GLuint fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceString, &fragmentLength);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus==GL_FALSE)
	{
		fprintf(stderr, "ERROR: Could not compile fragment shader %s\n", sourceDirectory.c_str());
		GLint logSize=0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		char* compileLog=(char*)malloc(logSize*sizeof(char));
		glGetShaderInfoLog(fragmentShader, logSize, &logSize, compileLog);
		fputs(compileLog, stderr);
		free(compileLog);
	}

	shaderProgram=glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	modelLocation=glGetUniformLocation(shaderProgram, "model");
	viewLocation=glGetUniformLocation(shaderProgram, "view");
	projectionLocation=glGetUniformLocation(shaderProgram, "proj");
	mvpLocation=glGetUniformLocation(shaderProgram, "mvp");

}

void Shader::Delete()
{
	glDeleteProgram(shaderProgram);
}

void Shader::SetMVPMatrix(OVR::Matrix4f mvp)
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp.Transposed().M[0][0]);
}

void Shader::SetViewMatrix(mat4 view)
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.m);
}

void Shader::SetProjMatrix(mat4 projection)
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.m);
}

void Shader::SetUniforms(mat4 model, mat4 view, mat4 projection)
{
	
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.m);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.m);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection.m);
}

Shader& Shader::operator=(const Shader& rhs)
{
	vertexShaderSource=rhs.vertexShaderSource;
	fragmentShaderSource=rhs.fragmentShaderSource;
	shaderProgram=rhs.shaderProgram;
	viewLocation=rhs.viewLocation;
	modelLocation=rhs.modelLocation;
	projectionLocation=rhs.projectionLocation;
}
void Shader::Use()
{
	glUseProgram(shaderProgram);
}

void Shader::LoadFromText(std::string sourceDirectory)
{
	//char* source=const_cast<char*>(sourceDirectory);
	long fileLength;

	std::string vertSourceFile=sourceDirectory;
	std::string fragSourceFile=sourceDirectory;
	vertSourceFile.append(".vert");
	fragSourceFile.append(".frag");

	//char* vertSourceFile=strcat(source, vert);
	//char* fragSourceFile=strcat(source, frag);

	FILE* vertFilePointer=fopen(vertSourceFile.c_str(), "r");
	if(vertFilePointer==NULL)
	{
		fprintf(stderr, "ERROR: could not load file %s\n", vertSourceFile.c_str());
	}
	fseek(vertFilePointer, 0, SEEK_END);
	fileLength=ftell(vertFilePointer);
	vertexShaderSource.resize(fileLength);
	fseek(vertFilePointer, 0, SEEK_SET);
	fread(&vertexShaderSource[0], sizeof(char), fileLength, vertFilePointer);
	fclose(vertFilePointer);



	FILE* fragFilePointer=fopen(fragSourceFile.c_str(), "r");
	if(fragFilePointer==NULL)
	{
		fprintf(stderr, "ERROR: could not load file %s\n", fragSourceFile.c_str());
	}
	fseek(fragFilePointer,0,SEEK_END);
	fileLength=ftell(fragFilePointer);
	fragmentShaderSource.resize(fileLength);
	fseek(fragFilePointer, 0, SEEK_SET);
	fread(&fragmentShaderSource[0], sizeof(char), fileLength, fragFilePointer);
	fclose(fragFilePointer);



}

Shader::~Shader()
{
	//free(fragmentShaderSource);
	//free(vertexShaderSource);
}
/*
const char* Shader::LoadShader(const char* source)
{
	FILE *fptr;
	long length;
	char *buf;

	fptr=fopen(source, "rb");
	if(!fptr) return NULL;
	fseek(fptr, 0, SEEK_END);
	length=ftell(fptr);
	buf=(char*)malloc(length+1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length];
	return buf;
}*/