#version 330 core
layout(location = 0)in vec3 fPos;
layout(location = 1)in vec2 tPos;
layout(location = 2)in vec3 nNormal;

out vec3 FragPos;
out vec2 TexturePos;
out vec3 Normal;
uniform mat4 proj, view, model;

void main()
{
	gl_Position = proj * view * model * vec4(fPos, 1.0f);
	TexturePos = tPos;
	FragPos = vec3(model * vec4(fPos, 1.0f));
	Normal = mat3(transpose(model)) * nNormal;
}