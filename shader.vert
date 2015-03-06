#version 430

layout(location=0) in vec3 position;
layout(location=1) in vec3 color;

uniform mat4 model, view, proj;

out vec3 outColor;
//out float dist;

void main()
{
	outColor=color;
	gl_Position=proj*view*model*vec4(position, 1.0);
	//dist=position.z; //1.0-(-pos_eye.z/10.0)
}