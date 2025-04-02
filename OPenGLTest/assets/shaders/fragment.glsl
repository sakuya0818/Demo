#version 460 core
out vec4 FragColor;

uniform sampler2D sampler;
uniform sampler2D sampler1;

in vec2 uv;

void main()
{
	vec4 aColor = texture(sampler, uv);
	vec4 bColor = texture(sampler1, uv);
	vec4 finalColor = aColor * 0.5 + bColor * 0.5;
	FragColor = vec4(finalColor.rgb, 1.0);
}