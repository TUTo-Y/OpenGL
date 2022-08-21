/*
*   程序名称 : OpenGL辅助库
*   程序作者 : TUTo
*   
*   创建时间 : 22-4-23
*   修改时间 : 22-5-1
*/
/*
*   struct GLTOCamera 
*       @在使用前应使用当初始化
*       @在每次刷新帧时必须使用gltoCameraTime更新时间用于计算速度
*   
*   struct Program
*       @Program允许不初始化, 调用CreateProgram创建一个着色器
*       @CreateProgram结束时会使用glUseProgram
*       @SetUniform设置Uniform
*   UniformID为program->Uniform[enum]
*/


/* 宏定义 */
#ifndef GLTO_H_
#define GLTO_H_





/* 
*   include
*   包含文件应当在 /include
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "control.h"    // 控制头
#include "stb_image.h"  // 图片读取
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "kazmath/kazmath.h"





/* 摄像机结构体 */
struct GLTOCamera
{
    /* 坐标 */
    kmVec3 pos,
           front,
           up;
    kmVec3 center; //pos + front
    kmMat4 lookAt;
    /* 坐标移动 */
    float blank_time, last_time;
    /* 镜头移动 */
    float lastx, lasty;
    BOOL frist;
    float yaw, pitch;
    /* 移动属性 */
    float sen;      // 灵敏度
    float speed;    // 坐标移动
};

/* 着色器结构体 */
struct GLTOProgram
{
    /*着色器程序 ID*/
    unsigned int Program;
    /*Uniform ID*/
    int* Uniform;
};





/**
 *  初始化摄像机
 *
 *  \param Camera : 摄像机结构体指针
 *
 *  \returns NULL
 */
void gltoCameraInit(struct GLTOCamera* Camera);

/**
 *  更新时间, 必须在每次更新帧时调用
 *
 *  \param Camera : 摄像机结构体指针
 *
 *  \returns NULL
 */
void gltoCameraTime(struct GLTOCamera *Camera);

/**
 *  移动, speed为移动速度, 模型阶段为4.0f
 *
 *  \param Camera   : 摄像机结构体指针
 *  \param Set      : 必须为GLFW_KEY_W,GLFW_KEY_S,GLFW_KEY_A,GLFW_KEY_D中的一个
 *
 *  \returns NULL
 */
void gltoCameraMove(struct GLTOCamera* Camera, short Set);

/**
 *  镜头旋转
 *
 *  \param Camera: 摄像机结构体指针
 *  \param float : 鼠标坐标X
 *  \param float : 鼠标坐标Y
 *
 *  \returns NULL
 */
void gltoCameraRotate(struct GLTOCamera* Camera, float xpos, float ypos);

/**
 *  更新Camera结构体中的lookAt和Center,并返回一个lookAt
 *
 *  \param Camera : 摄像机结构体指针
 *
 *  \returns lookAt
 */
kmMat4* gltoCameralookAt(struct GLTOCamera* Camera);

/**
 * 创建着色器程序，自动初始化Program,自动调用glUseProgram
 *
 * \param program : 着色器程序结构体指针
 * \param ShaderFile : 可变参
 *                      @ShaderFile[0]:顶点着色器文件名
 *                      @ShaderFile[1]:片元着色器文件名
 *
 * \returns BOOL判断是否创建成功
 */
BOOL gltoCreateProgram(struct GLTOProgram* program, ...);

/**
 * 设置着色器Uniform,用program->Uniform[enum]代替glGetUniformLocation(Program, "enum")
 *
 * \param program:着色器程序结构体指针
 * \param int:Uniform数组地址
 * \param const char*:Uniform名
 * \param 最后一个参数必须是-1
 *
 * \returns void
 */
void gltoSetUniform(struct GLTOProgram* program, ...);

/**
 * 删除着色器程序
 *
 * \param program:着色器程序结构体指针
 *
 * \returns void
 */
void gltoDeleteProgram(struct GLTOProgram* program);

/**
 * 创建一个2D纹理
 *
 * \param Texture:纹理ID指针
 * \param File:文件名
 *
 * \returns 纹理是否创建成功
 */
BOOL gltoCreateTexture2D(unsigned int *Texture, const char*File);

/**
 * 创建一个天空盒子
 *
 * \param Texture : 纹理ID指针
 * \param ...File : 文件名
 *                  @right
 *                  @left
 *                  @top
 *                  @bottom
 *                  @front
 *                  @back
 *
 * \returns 纹理是否创建成功
 */
BOOL gltoCreateTextureCube(unsigned int* Texture, ...);


#endif  // GLTO_H_
