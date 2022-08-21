#include <Model.h>
void LoadTexture(unsigned int* Texture, struct aiMaterial* Mat, unsigned int Type, const char* Directory);
BOOL TextureFromFile(unsigned int* Texture, const char* File, const char* Directory);
/*绘制*/
void ModelDraw(struct Model* Model)
{
    glUseProgram(Model->Program);
    for (unsigned int i = 0; i < Model->scene->mNumMeshes; i++)
    {
        struct Texture* Texture;
        unsigned int j = 0, k = 0;
        Texture = &Model->Texture[Model->scene->mMeshes[i]->mMaterialIndex];
        for(j = 0; j<Texture->DiffuseNumber; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, Texture->Texture[j + k]);
        }
        k += j;

        for (j = 0; j < Texture->SpecularNumber; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j + Model->DiffuseNumber);
            glBindTexture(GL_TEXTURE_2D, Texture->Texture[j + k]);
        }
        k += j;

        for (j = 0; j < Texture->HeightNumber; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j + Model->DiffuseNumber + Model->SpecularNumber);
            glBindTexture(GL_TEXTURE_2D, Texture->Texture[j + k]);
        }
        k += j;

        for (j = 0; j < Texture->AmbientNumber; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j + Model->DiffuseNumber + Model->SpecularNumber + Model->HeightNumber);
            glBindTexture(GL_TEXTURE_2D, Texture->Texture[j + k]);
        }

        // 绘制
        glBindVertexArray(Model->Mesh[i].VAO);
        glDrawElements(GL_TRIANGLES, Model->Mesh[i].Number, GL_UNSIGNED_INT, 0);
    }
}

/*读取模型数据*/
BOOL ModelLoad(struct Model* Model, const char* FileStr, unsigned int Program)
{
    unsigned int i = 0, j = 0;
    unsigned int* T;
    char Name[50];
    /*初始化Model*/
    memset(Model, 0, sizeof(struct Model));
    Model->Program = Program;

    /*读取数据*/
    memcpy(Model->scene,
        aiImportFile(FileStr,
        aiProcess_Triangulate   |   // 所有的图元形状变换为三角形
        aiProcess_GenNormals    |   // 创建法向量
        aiProcess_OptimizeMeshes),    // 合并图元
        //aiProcess_FlipUVs     |   // 翻转
        sizeof(Model->scene)
    );
    /*片段读取结果*/
    if (!Model->scene) 
    {
        printf("读取模型 : %s 错误!\n", FileStr);
        return FALSE;
    }

    /*获取模型文件目录*/
    i = strlen(FileStr);
    while (i >= 0 && (FileStr[i] != '/') && (FileStr[i] != '\\'))
        i--;
    memcpy(Model->Directory, FileStr, sizeof(char) * (i + 1));
    Model->Directory[i + 1] = '\0';


    /*读取纹理*/
    Model->Texture = (struct Texture*)malloc(Model->scene->mNumMaterials * sizeof(struct Texture));
    memset(Model->Texture, 0, _msize(Model->Texture));
#ifdef _DEBUG
    Model->DiffuseNumber = 0;
    Model->SpecularNumber = 0;
    Model->HeightNumber = 0;
    Model->AmbientNumber = 0;
#endif
    for (i = 0; i < Model->scene->mNumMaterials; i++)
    {
        /*设置各个纹理类型数量*/
        Model->Texture[i].DiffuseNumber = aiGetMaterialTextureCount(Model->scene->mMaterials[i], aiTextureType_DIFFUSE);
        Model->DiffuseNumber = Model->Texture[i].DiffuseNumber > Model->DiffuseNumber ? Model->Texture[i].DiffuseNumber : Model->DiffuseNumber;
        
        Model->Texture[i].SpecularNumber = aiGetMaterialTextureCount(Model->scene->mMaterials[i], aiTextureType_SPECULAR);
        Model->SpecularNumber = Model->Texture[i].SpecularNumber > Model->SpecularNumber ? Model->Texture[i].SpecularNumber : Model->SpecularNumber;
        
        Model->Texture[i].HeightNumber = aiGetMaterialTextureCount(Model->scene->mMaterials[i], aiTextureType_HEIGHT);
        Model->HeightNumber = Model->Texture[i].HeightNumber > Model->HeightNumber ? Model->Texture[i].HeightNumber : Model->HeightNumber;
        
        Model->Texture[i].AmbientNumber = aiGetMaterialTextureCount(Model->scene->mMaterials[i], aiTextureType_AMBIENT);
        Model->AmbientNumber = Model->Texture[i].AmbientNumber > Model->AmbientNumber ? Model->Texture[i].AmbientNumber : Model->AmbientNumber;

        /*读取纹理*/
        Model->Texture[i].Texture = (unsigned int*)malloc(
            (   Model->Texture[i].DiffuseNumber +
                Model->Texture[i].SpecularNumber +
                Model->Texture[i].HeightNumber +
                Model->Texture[i].AmbientNumber)
            * sizeof(unsigned int));
        memset(Model->Texture[i].Texture, 0, _msize(Model->Texture[i].Texture));
        T = Model->Texture[i].Texture;
        LoadTexture(T, Model->scene->mMaterials[i], aiTextureType_DIFFUSE, Model->Directory);
        T += Model->Texture[i].DiffuseNumber;
        LoadTexture(T, Model->scene->mMaterials[i], aiTextureType_SPECULAR, Model->Directory);
        T += Model->Texture[i].SpecularNumber;
        LoadTexture(T, Model->scene->mMaterials[i], aiTextureType_HEIGHT, Model->Directory);
        T += Model->Texture[i].HeightNumber;
        LoadTexture(T, Model->scene->mMaterials[i], aiTextureType_AMBIENT, Model->Directory);
        T += Model->Texture[i].AmbientNumber;
    }

    /*绑定纹理*/
    j = 0;
    for (i = 0; i < Model->DiffuseNumber; i++)
    {
        memset(Name, 0, sizeof(Name));
        strcpy(Name, "Texture_Diffuse");
        Name[sizeof("Texture_Diffuse") - 1] = i + 48;
        Name[sizeof("Texture_Diffuse")] = '\0';
        glUniform1i(glGetUniformLocation(Program, Name), i + j);
    }
    j += i;
    for (i = 0; i < Model->SpecularNumber; i++)
    {
        memset(Name, 0, sizeof(Name));
        strcpy(Name, "Texture_Specular");
        Name[sizeof("Texture_Specular") - 1] = i + 48;
        Name[sizeof("Texture_Specular")] = '\0';
        glUniform1i(glGetUniformLocation(Program, Name), i + j);
    }
    j += i;
    for (i = 0; i < Model->HeightNumber; i++)
    {
        memset(Name, 0, sizeof(Name));
        strcpy(Name, "Texture_Height");
        Name[sizeof("Texture_Height") - 1] = i + 48;
        Name[sizeof("Texture_Height")] = '\0';
        glUniform1i(glGetUniformLocation(Program, Name), i + j);
    }
    j += i;
    for (i = 0; i < Model->AmbientNumber; i++)
    {
        memset(Name, 0, sizeof(Name));
        strcpy(Name, "Texture_Ambient");
        Name[sizeof("Texture_Ambient") - 1] = i + 48;
        Name[sizeof("Texture_Ambient")] = '\0';
        glUniform1i(glGetUniformLocation(Program, Name), i + j);
    }


    /*设置网格*/
    Model->Mesh = (struct Mesh*)malloc(Model->scene->mNumMeshes * sizeof(struct Mesh));
    memset(Model->Mesh, 0, _msize(Model->Mesh));
    for (i = 0; i < Model->scene->mNumMeshes; i++)
    {
        struct Mesh* Mesh;
        Mesh = &Model->Mesh[i];
        glGenVertexArrays(1, &Mesh->VAO);
        glGenBuffers(1, &Mesh->EBO);
        glGenBuffers(1, &Mesh->Position);
        glGenBuffers(1, &Mesh->Normal);
        glGenBuffers(1, &Mesh->TexturePos);
        glGenBuffers(1, &Mesh->Tangent);
        glGenBuffers(1, &Mesh->Bitangent);
        glBindVertexArray(Mesh->VAO);
        // EBO
        Mesh->Number = 0;
        for (j = 0; j < Model->scene->mMeshes[i]->mNumFaces; j++)
            Mesh->Number += Model->scene->mMeshes[i]->mFaces[j].mNumIndices;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh->Number * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
        char* ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        for (j = 0; j < Model->scene->mMeshes[i]->mNumFaces; j++)
        {
            memcpy(ptr,
                Model->scene->mMeshes[i]->mFaces[j].mIndices,
                Model->scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(unsigned int));
            ptr += (int)(Model->scene->mMeshes[i]->mFaces[j].mNumIndices * sizeof(unsigned int));
        }
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        
        // Vertex
        glBindBuffer(GL_ARRAY_BUFFER, Mesh->Position);
        glBufferData(GL_ARRAY_BUFFER, Model->scene->mMeshes[i]->mNumVertices * sizeof(struct aiVector3D), 
            Model->scene->mMeshes[i]->mVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct kmVec3), (void*)0);
        // Normal
        glBindBuffer(GL_ARRAY_BUFFER, Mesh->Normal);
        glBufferData(GL_ARRAY_BUFFER, Model->scene->mMeshes[i]->mNumVertices * sizeof(struct aiVector3D),
            Model->scene->mMeshes[i]->mNormals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct kmVec3), (void*)0);
        // TexturePos
        glBindBuffer(GL_ARRAY_BUFFER, Mesh->TexturePos);
        glBufferData(GL_ARRAY_BUFFER, Model->scene->mMeshes[i]->mNumVertices * sizeof(struct aiVector3D),
            Model->scene->mMeshes[i]->mTextureCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct kmVec3), (void*)0);
        // Tangent
        glBindBuffer(GL_ARRAY_BUFFER, Mesh->Tangent);
        glBufferData(GL_ARRAY_BUFFER, Model->scene->mMeshes[i]->mNumVertices * sizeof(struct aiVector3D),
            Model->scene->mMeshes[i]->mTangents, GL_STATIC_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct kmVec3), (void*)0);
        // Bitangent
        glBindBuffer(GL_ARRAY_BUFFER, Mesh->Bitangent);
        glBufferData(GL_ARRAY_BUFFER, Model->scene->mMeshes[i]->mNumVertices * sizeof(struct aiVector3D),
            Model->scene->mMeshes[i]->mBitangents, GL_STATIC_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(struct kmVec3), (void*)0);
    }
    return TRUE;
}

void LoadTexture(unsigned int* Texture, struct aiMaterial* Mat, unsigned int Type, const char * Directory)
{
    struct aiString Path = { 0 };
    unsigned int i = 0;
    for (i = 0; i < aiGetMaterialTextureCount(Mat, Type); i++)
    {
        memset(&Path, 0, sizeof(Path));
        aiGetMaterialTexture(Mat, Type, i, &Path, NULL, NULL, NULL, NULL, NULL, NULL);
        TextureFromFile(Texture + i, Path.data, Directory);
    }
}

/**
 * 读取Texture
 *
 * \param Texture:纹理ID指针
 * \param File:文件
 * \param Directory:文件目录必须以\或/结尾
 *
 * \returns BOOL判断是否创建成功
 */
BOOL TextureFromFile(unsigned int* Texture, const char* File, const char* Directory)
{
    char P[MAX_PATH] = { 0 };
    /*字符串*/
    strcpy(P, Directory);
    strcat(P, File);

    return CreateTexture2D(Texture, P);
}
