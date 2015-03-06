#version 430

in vec3 outColor;
//in float dist;

out vec4 fragColor;

void main()
{
	fragColor=vec4(outColor, 1.0);
	//fragColor.xyz*=dist;

}