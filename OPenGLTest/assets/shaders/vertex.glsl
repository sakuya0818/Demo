#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 position = vec4(aPos, 1.0);
	position = projection * view * transform * position;
	gl_Position = position;
	//gl_Position = vec4(aPos, 1.0);
	uv = aUV;
}