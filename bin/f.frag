#version 330 core
in vec3 FragPos;
uniform samplerCube Texture;
out vec4 FragColor;
void main()
{
	FragColor = texture(Texture, FragPos);
}