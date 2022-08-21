#version 330 core
in vec3 FragPos;			//片元坐标
in vec2 TexturePos;			//纹理坐标
in vec3 Normal;				//法向量

uniform sampler2D Texture_Diffuse0;
uniform sampler2D Texture_Diffuse1;
uniform sampler2D Texture_Diffuse2;
uniform sampler2D Texture_Diffuse3;
uniform sampler2D Texture_Diffuse4;
uniform sampler2D Texture_Specular0;
uniform sampler2D Texture_Specular1;
uniform sampler2D Texture_Specular2;
uniform sampler2D Texture_Specular3;
uniform sampler2D Texture_Specular4;

uniform float Spec; //光圈
uniform vec3 ViewPos;//位置
struct _Light
{
	vec3 Position;			//灯的位置
	
	vec3 Ambient;			//周围颜色
	vec3 Diffuse;			//漫反射颜色
	vec3 Specular;			//高光颜色
};
uniform _Light Light;

out vec4 Color;
void main()
{
	/*合成纹理*/
	vec4 Texture_Diffuse = (texture(Texture_Diffuse0, TexturePos));
	vec4 Texture_Specular = (texture(Texture_Specular0, TexturePos));
	/*环境光照*/
	vec4 Ambient = vec4(Light.Ambient, 1.0f) * Texture_Diffuse;
	/*漫反射*/
	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(Light.Position - FragPos);
	vec4 Diffuse = vec4(max(dot(Norm, LightDir), 0.0f) * Light.Diffuse, 1.0f);
		Diffuse *= Texture_Diffuse;
	/*镜面反射*/
	vec3 Reflect = reflect(-LightDir, Norm);
	vec3 ViewDir = normalize(ViewPos - FragPos);
	vec4 Specular = vec4(Spec * pow(max(dot(ViewDir, Reflect), 0.0f), 32) * Light.Specular, 1.0f);
	Specular *= Texture_Specular;
	
	/*最终颜色*/
	Color = Ambient + Diffuse + Specular;
	//Color = vec4(1.0f, 0.8f, 0.4f, 1.0f);
}

