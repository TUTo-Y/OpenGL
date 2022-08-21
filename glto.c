/*
*   程序名称 : OpenGL辅助库
*   程序作者 : TUTo
*   
*   创建时间 : 22-4-23
*   修改时间 : 22-5-1
*/

/* 
    包含头文件和image读取
*/
#define STB_IMAGE_IMPLEMENTATION
#include "glto.h"

void gltoCameraInit(struct GLTOCamera* Camera)
{
    /* 初始化结构体 */
    memset(Camera, 0, sizeof(struct GLTOCamera));

    /* 初始化坐标 */
    Camera->pos     = (kmVec3){ 0.0f, 0.0f,  3.0f };
    Camera->front   = (kmVec3){ 0.0f, 0.0f, -1.0f };
    Camera->up      = (kmVec3){ 0.0f, 1.0f,  0.0f };
    Camera->center  = (kmVec3){ 0.0f, 0.0f,  0.0f };
    kmMat4Identity(&Camera->lookAt);
    /* 初始化坐标移动 */
    Camera->blank_time = 0.0f;
    Camera->last_time = 0.0f;
    /* 初始化镜头移动 */
    Camera->lastx = 0.0f;
    Camera->lasty = 0.0f;
    Camera->frist = TRUE;
    Camera->yaw = -90.0f;
    Camera->pitch = 0.0f;
    /* 初始化移动属性和镜头移动 */
    Camera->sen = 0.15f;  // 镜头移动
    Camera->speed = 4.0f; // 坐标移动
}

void gltoCameraTime(struct GLTOCamera* Camera)
{
    /* 设置计算时间差 */
    Camera->blank_time = (float)glfwGetTime() - Camera->last_time;
    /* 更新时间 */
    Camera->last_time += Camera->blank_time;
}

void gltoCameraMove(struct GLTOCamera* Camera, short Set)
{
    /* 设置移动距离 */
    float Speed = Camera->speed * Camera->blank_time;
    kmVec3 Temp;
    if (Set == GLFW_KEY_W)  // 前进
    {
        Camera->pos.x += Speed * Camera->front.x;
        Camera->pos.y += Speed * Camera->front.y;
        Camera->pos.z += Speed * Camera->front.z;
    }
    if (Set == GLFW_KEY_S) // 后退
    {
        Camera->pos.x -= Speed * Camera->front.x;
        Camera->pos.y -= Speed * Camera->front.y;
        Camera->pos.z -= Speed * Camera->front.z;
    }
    if (Set == GLFW_KEY_A) // 向左
    {
        kmVec3Normalize(&Temp, kmVec3Cross(&Temp, &Camera->front, &Camera->up));
        Camera->pos.x -= Temp.x * Speed;
        Camera->pos.y -= Temp.y * Speed;
        Camera->pos.z -= Temp.z * Speed;
    }
    if (Set == GLFW_KEY_D) // 向右
    {
        kmVec3Normalize(&Temp, kmVec3Cross(&Temp, &Camera->front, &Camera->up));
        Camera->pos.x += Temp.x * Speed;
        Camera->pos.y += Temp.y * Speed;
        Camera->pos.z += Temp.z * Speed;
    }
}

void gltoCameraRotate(struct GLTOCamera* Camera, float xpos, float ypos)
{
    /* 第一次移动镜头 */
    if (Camera->frist)
    {
        Camera->lastx = xpos;
        Camera->lasty = ypos;
        Camera->frist = FALSE;
    }

    /* 计算角度 */
    Camera->yaw += Camera->sen * (xpos - Camera->lastx);
    Camera->pitch += Camera->sen * (Camera->lasty - ypos);
    /* 更新鼠标位置 */
    Camera->lastx = xpos;
    Camera->lasty = ypos;

    /* 避免角度大于90 */
    if (Camera->pitch > 89.0f)
        Camera->pitch = 89.0f;
    else if (Camera->pitch < -89.0f)
        Camera->pitch = -89.0f;

    /* 设置摄像机 */
    Camera->front.x = (float)cos(kmDegreesToRadians(Camera->yaw)) * (float)cos(kmDegreesToRadians(Camera->pitch));
    Camera->front.y = (float)sin(kmDegreesToRadians(Camera->pitch));
    Camera->front.z = (float)sin(kmDegreesToRadians(Camera->yaw)) * (float)cos(kmDegreesToRadians(Camera->pitch));
    kmVec3Normalize(&Camera->front, &Camera->front);
}

kmMat4* gltoCameralookAt(struct GLTOCamera* Camera)
{
    /* 合成center */
    Camera->center.x= Camera->pos.x + Camera->front.x;
    Camera->center.y = Camera->pos.y + Camera->front.y;
    Camera->center.z = Camera->pos.z + Camera->front.z;

    return kmMat4LookAt(kmMat4Identity(&Camera->lookAt), &Camera->pos, &Camera->center, &Camera->up);
}

BOOL gltoCreateProgram(struct GLTOProgram* program, ...)
{
    const char* ShaderName[2] = {"顶点", "片元"};

    unsigned int Shader = 0;    // 着色器子程序
    char *ShaderFile    = NULL; // 着色器源码文件
    char error[512]     = {0};  // 错误内容
    va_list list;               // 可变参
    FILE* File          = NULL; // 着色器文件指针
    char *Str           = 0;    // 着色器源码
    int Size            = 0;    // 着色器文件大小

    /* 使用可变参 */
    va_start(list, program);

    /* 初始化结构体 */
    memset((void*)program, 0, sizeof(struct GLTOProgram));

    /* 创建顶点着色器和片段着色器 */
    program->Program = glCreateProgram();
    for (unsigned short i = 0; i < 2; i++)
    {
        /* 获取着色器文件名 */
        ShaderFile = va_arg(list, char*);
        /* 打开文件 */
        if (!(File = fopen(ShaderFile, "r")))
        {
            //错误处理
            printf("\n%s着色器文件读取错误 : %s\n", ShaderName[i], ShaderFile);
            glDeleteProgram(program->Program);
            va_end(list);
            return FALSE;
        }
        /* 读取内存 */
        fseek(File, 0L, SEEK_END);
        Str = (char*)malloc((Size = ftell(File)) + 1);
        rewind(File);
        memset(Str, 0, Size);
        fread(Str, Size, 1, File);
        fclose(File);
        /* 编译着色器 */
#if GL_VERTEX_SHADER - 1 == GL_FRAGMENT_SHADER
        Shader = glCreateShader(GL_VERTEX_SHADER - i);
#else
        if(i)
            Shader = glCreateShader(GL_VERTEX_SHADER);
        else
            Shader = glCreateShader(GL_FRAGMENT_SHADER);
#endif
        glShaderSource(Shader, 1, (const char**)&Str, NULL);
        glCompileShader(Shader);
        free(Str);

        /* 编译错误 */
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &Size);
        if (!Size)
        {
            glGetShaderInfoLog(Shader, 512, NULL, error);
            printf("\n%s着色器:%s编译错误 : \n%s\n", ShaderName[i], ShaderFile, error);
            glDeleteProgram(program->Program);
            va_end(list);
            return FALSE;
        }

        /* 贴附程序 */
        glAttachShader(program->Program, Shader);
        glDeleteShader(Shader);
    }

    /* 链接程序 */
    glLinkProgram(program->Program);
    glUseProgram(program->Program);
    va_end(list);
    return TRUE;
}

void gltoSetUniform(struct GLTOProgram* program, ...)
{
    va_list arg_ptr;
    int i = 0;
    int v;
    char* c;

    /* 获取Uniform数量 */
    va_start(arg_ptr, program);
    while (va_arg(arg_ptr, int) != -1)
    {
        va_arg(arg_ptr, char*);
        i++;
    }
    va_end(arg_ptr);

    /* 创建Uniform */
    va_start(arg_ptr, program);
    program->Uniform = (int*)malloc(sizeof(int) * i);
    while (i--)
    {
        v = va_arg(arg_ptr, int);
        c = va_arg(arg_ptr, char*);
        *(program->Uniform + v) = glGetUniformLocation(program->Program, c);
    }
    va_end(arg_ptr);
}

void gltoDeleteProgram(struct GLTOProgram *program)
{
    glDeleteProgram(program->Program);
    free(program->Uniform);
    memset((void*)program, 0, sizeof(struct GLTOProgram));
}

BOOL gltoCreateTexture2D(unsigned int* Texture, const char* File)
{
    void* data = NULL;  //纹理数据
    int w, h, n;        //纹理信息
    /*读取文件数据*/
    if (data = stbi_load(File, &w, &h, &n, 0))
    {
        GLenum format;
        switch (n)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            printf("\n无法检测图片文件的颜色格式, 错误文件 : %s\n", File);
            return FALSE;
            break;
        }
        /*创建纹理*/
        glGenTextures(1, Texture);
        glBindTexture(GL_TEXTURE_2D, *Texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(data);
#ifdef _DEBUG
        printf("读取图像成功, 文件名为 : %s\n", File);
#endif
        return TRUE;
    }
    else
    {
        printf("\n打开文件错误, 错误文件 : %s\n", File);
        return FALSE;
    }
}

BOOL gltoCreateTextureCube(unsigned int* Texture, ...)
{
    void* data = NULL;  //纹理数据
    int w, h, n;        //纹理信息
    char *fstr;         // 文件名
    va_list list;

    /* 使用可变参 */
    va_start(list, Texture);

    /*创建纹理*/
    glGenTextures(1, Texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *Texture);

    for (unsigned int i = 0; i < 6; i++)
    {
        fstr = va_arg(list, char*);
        if (data = stbi_load(fstr, &w, &h, &n, 0))
        {
            GLenum format;
            switch (n)
            {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                printf("\n无法检测图片文件的颜色格式, 错误文件 : %s\n", fstr);
                glDeleteTextures(1, Texture);
                va_end(list);
                return FALSE;
                break;
            }
            /*加载*/
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
#ifdef _DEBUG
            printf("读取图像成功, 文件名为 : %s\n", fstr);
#endif
        }
        else
        {
            printf("\n打开文件错误, 错误文件 : %s\n", fstr);
            glDeleteTextures(1, Texture);
            va_end(list);
            return FALSE;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    va_end(list);
    return TRUE;
}

