// C include
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <control.h>

// Other include
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <kazmath/kazmath.h>

// My include
#include <glto.h>

/* 全局数据 */
int Window_W = 800, Window_H = 600;
struct GLTOCamera Camera;

/* 鼠标响应函数 */
void cursorFun(GLFWwindow*, double, double);
void cursorFun(GLFWwindow* Window, double x, double y)
{
#ifdef _DEBUG
    Window = NULL;
#endif
    gltoCameraRotate(&Camera, (float)x, (float)y);
}

/* 键盘响应函数 */
void keyFun(GLFWwindow* Window)
{
    gltoCameraTime(&Camera);
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window, GLFW_TRUE);
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        gltoCameraMove(&Camera, GLFW_KEY_W);
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        gltoCameraMove(&Camera, GLFW_KEY_S);
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        gltoCameraMove(&Camera, GLFW_KEY_A);
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        gltoCameraMove(&Camera, GLFW_KEY_D);
}


// 渲染
void Play(GLFWwindow*);
void Play(GLFWwindow*Window)
{
    /*Model*/
    /* 正方体(逆时针) */
    float Point[] = {
	    /*位置*/               /*纹理坐标*/   /*法线*/
    	 /*右面*/
	     1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
	     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	     1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

    	/*左面*/
	    -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    	-1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    	-1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,

	    /*上面*/
    	 1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
       	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    	 1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,

	    /*下面*/
    	-1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    	 1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    	-1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    	-1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,

	    /*正面*/
	     1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

    	 /*后面*/
    	-1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
    	 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,
    	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
    	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
    	-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
    	-1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f
    };
    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point), Point, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /*创建着色器*/
    enum {proj, view, model, Spec, ViewPos,
        Light_Position, Light_Ambient, Light_Diffuse, Light_Specular
    };
    struct GLTOProgram program = {0};
    gltoCreateProgram(&program, "Vertex.vert", "Fragment.frag");
    gltoSetUniform(&program,
        proj, "proj", view, "view", model, "model", Spec, "Spec", ViewPos, "ViewPos",
        Light_Position, "Light.Position", Light_Ambient, "Light.Ambient", Light_Diffuse, "Light.Diffuse", Light_Specular, "Light.Specular",
        -1);
    /*设置Uniform数据*/
    struct kmMat4 Mat4;

    glUniformMatrix4fv(program.Uniform[proj], 1, GL_FALSE, (float*)kmMat4PerspectiveProjection(kmMat4Identity(&Mat4), 45.0f, (float)(Window_W / Window_H), 0.1f, 500.0f));
    glUniformMatrix4fv(program.Uniform[model], 1, GL_FALSE, (float*)kmMat4Translation(kmMat4Identity(&Mat4), 0.0f, 0.0f, -2.0f));
    
    glUniform1f(program.Uniform[Spec], 0.5f);
    glUniform3f(program.Uniform[Light_Position], -1.0f, 2.0f, 2.0f);
    glUniform3f(program.Uniform[Light_Ambient], 0.4f, 0.4f, 0.4f);
    glUniform3f(program.Uniform[Light_Diffuse], 0.65f, 0.65f, 0.65f);
    glUniform3f(program.Uniform[Light_Specular], 1.0f, 1.0f, 1.0f);

    unsigned int Texture;
    gltoCreateTexture2D(&Texture, "1.jpg");

    /*天空盒*/
    float skyPoint[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyPoint), skyPoint, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    struct GLTOProgram Tex;
    gltoCreateProgram(&Tex, "v.vert", "f.frag");
    gltoSetUniform(&Tex, proj, "proj", view, "view",
        -1);
    glUniformMatrix4fv(Tex.Uniform[proj], 1, GL_FALSE, (float*)kmMat4PerspectiveProjection(kmMat4Identity(&Mat4), 45.0f, (float)(Window_W / Window_H), 0.1f, 500.0f));
    unsigned int skyTexture;

    glBindTexture(GL_TEXTURE_2D, 0);


    stbi_set_flip_vertically_on_load(FALSE);
    gltoCreateTextureCube(&skyTexture,
    #if 0
    "1.jpg",
    "1.jpg",
    "1.jpg",
    "1.jpg",
    "1.jpg",
    "1.jpg");
        #endif
    #if 1
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg");
#endif
    //glFrontFace(GL_CCW);
    //glCullFace(GL_FRONT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    while (!glfwWindowShouldClose(Window))
    {
        // 前处理
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        keyFun(Window);

        // 渲染
        glUseProgram(program.Program);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform3fv(program.Uniform[ViewPos], 1, (float*)&Camera.pos);
        glUniformMatrix4fv(program.Uniform[view], 1, GL_FALSE, (float*)gltoCameralookAt(&Camera));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LEQUAL);
        glUseProgram(Tex.Program);
        memcpy(&Mat4, gltoCameralookAt(&Camera),  sizeof(struct kmMat4));
        float *P;
        P = &Mat4.mat[0];
        *(P + 3) = 0;
        *(P + 7) = 0;
        *(P + 11) = 0;
        *(P + 12) = 0;
        *(P + 13) = 0;
        *(P + 14) = 0;
        *(P + 15) = 0;
        glUniformMatrix4fv(Tex.Uniform[view], 1, GL_FALSE, (float*)P);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
        glBindVertexArray(skyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        // 后处理
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

}

int main()
{
    /* 创建GLFW窗口 */
    glfwInit();
    gltoCameraInit(&Camera);
    stbi_set_flip_vertically_on_load(TRUE);
    GLFWwindow* Window = glfwCreateWindow(Window_W, Window_H, "Hello OpenGL!", 0, 0);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Window, cursorFun);
    glfwMakeContextCurrent(Window);

    /* 初始化OpenGL */
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    Play(Window);

    glfwDestroyWindow(Window);
    glfwTerminate();
    return 0;
}