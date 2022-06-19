#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "libs/common/GLShader.h"

#include "Vertex.h"
#include "DragonData.h"

#include <iostream>
//#include "Header.h"
#define totalFigure 2



// attention, ce define ne doit etre specifie que dans 1 seul fichier cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLShader g_TransformShader;

GLuint VBO;
GLuint IBO;
GLuint VAO;

GLuint TexID;
GLuint vbos[totalFigure];
GLuint ibos[totalFigure];
GLuint vaos[totalFigure];


const unsigned  int indices[] = {
    0, 1, 2  , // first triangle
    1,2,3
};

void loadTexFromFile(const char* filename) {

    //On initialise la texture
    glGenTextures(1, &TexID);
    glBindTexture(GL_TEXTURE_2D, TexID);

    // Filtrage bilineaire dans tous les cas (Minification et Magnification)
    // les coordonnees de texture sont limitees a [0 ; 1[
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int w, h;
    uint8_t* data = stbi_load(filename, &w, &h, nullptr, STBI_rgb_alpha);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
}

bool Initialise()
{
    GLenum ret = glewInit();

    g_TransformShader.LoadVertexShader("transform.vs");
    g_TransformShader.LoadFragmentShader("transform.fs");
    g_TransformShader.Create();

    const Vertex triangle[] = {
    {{-2.0f, -2.0f,0.0f}, {0.f, 0.f}, {255, 0, 0, 255}},   // sommet 0
    {{0.0f, 0.0f,1.0f},  {1.f, 0.f}, {0, 255, 0, 255}},   // sommet 1
    {{0.0f, -2.0f,1.5f},   {0.f, 1.f}, {0, 0, 255, 255}},    // sommet 2
    {{2.0f,  -2.0f, 0.0f}, {0.f, 1.f}, {0, 0, 255, 255}},   // Sommet 4
    }
    ;

    const int triangle_index[] = {
        0, 1, 2, 1, 2, 3
    };

    glGenBuffers(totalFigure, vbos);
    glGenBuffers(totalFigure, ibos);
    glGenVertexArrays(totalFigure, vaos);

    // je recommande de reinitialiser les etats a la fin pour eviter les effets de bord

    constexpr size_t stride = sizeof(Vertex);

    // 
    auto program = g_TransformShader.GetProgram();
    // VAO ---

    glBindVertexArray(vaos[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_index), triangle_index, GL_STATIC_DRAW);

    int loc_position = glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , GL_FALSE, stride, (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    //loadObj("cube.obj");

    //2eme objet
    constexpr size_t strides = sizeof(DragonVertex);// sizeof(float) * 5;
    glBindVertexArray(vaos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(DragonVertices), DragonVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DragonIndices), DragonIndices, GL_STATIC_DRAW);

    loc_position = glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , false, strides, (void*)offsetof(DragonVertex, position));

    int loc_uv = glGetAttribLocation(program, "a_texcoords");
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT
        , false, strides, (void*)offsetof(DragonVertex, uv));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    loadTexFromFile("dragon.png");



    return true;
}

void Terminate()
{
    glDeleteTextures(1, &TexID);

    for (int i = 0; i < sizeof(vaos) / sizeof(GLuint); i++) {
        glDeleteVertexArrays(1, &vaos[i]);
    }
    for (int i = 0; i < sizeof(vbos) / sizeof(GLuint); i++) {
        glDeleteBuffers(1, &vbos[i]);
    }
    for (int i = 0; i < sizeof(ibos) / sizeof(GLuint); i++) {
        glDeleteBuffers(1, &ibos[i]);
    }

    g_TransformShader.Destroy();
}

void Render(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto program = g_TransformShader.GetProgram();
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TexID);

    GLint textureLocation = glGetUniformLocation(program, "u_sampler");
    glUniform1i(textureLocation, 0);

    const float zNear = 0.1f;
    const float zFar = 100;
    const float aspect = float(width) / float(height);
    const float fov = 45.0f * M_PI / 180.0f;
    const float f = 1.0f / tanf(fov / 2.0f);
    const float projection[] = {
        f / aspect, 0.f, 0.f, 0.f,
        0.f, f, 0.f, 0.f,
        0.f, 0.f, ((zFar + zNear) / (zNear - zFar)), -1.f,
        0.f, 0.f, ((2 * zNear * zFar) / (zNear - zFar)), 0.f
    };



    GLint proj = glGetUniformLocation(program, "u_projection");
    glUniformMatrix4fv(proj, 1, false, projection);
    float translationX = 0.f;
    float translationY = 0.f;
    float translationZ = -30.f;

    
    const float translation[] = {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        translationX, translationY, translationZ, 1.f
    };

    const float translation2[] = {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        translationX, translationY,10.f, 1.f
    };

    GLint trans = glGetUniformLocation(program, "u_translation");
    glUniformMatrix4fv(trans, 1, false, translation);

    glBindVertexArray(vaos[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(vaos[1]);
    glDrawElements(GL_TRIANGLES, _countof(DragonVertices), GL_UNSIGNED_SHORT, 0);
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Affichage 3D", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    Initialise();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        Render(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Terminate();

    glfwTerminate();
    return 0;
}