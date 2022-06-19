#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "libs/common/GLShader.h"

#include "Vertex.h"
#include "DragonData.h"
#include <vector>

#include <iostream>
#include "Header.h"
#define totalFigure 3



// Ne définir qu'une fois
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLShader g_TransformShaders[totalFigure];

int figSizes[totalFigure];

GLuint TexID;
GLuint vbos[totalFigure];
GLuint ibos[totalFigure];
GLuint vaos[totalFigure];
std::vector<int> indice;
std::vector<Vertex> fig = loadObj("cube.obj", indice);

std::vector<int> indice_fig2;
std::vector<Vertex> fig2 = loadObj("leaftree.obj", indice_fig2);

const unsigned  int indices[] = {
    0, 1, 2  , // first triangle
    1,2,3
};

void loadTexFromFile(const char* filename) {

    glGenTextures(1, &TexID);
    glBindTexture(GL_TEXTURE_2D, TexID);

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

void printVector(std::vector<Vertex> vV) {
    std::cout << "{";
    for (Vertex v : vV) {
        std::cout << "{" << v.position.x << " ," << v.position.y << " ," << v.position.z << "},\n";
    }
    std::cout << "}";
}

bool Initialise()
{
    GLenum ret = glewInit();

    g_TransformShaders[0].LoadVertexShader("transform.vs");
    g_TransformShaders[0].LoadFragmentShader("transform.fs");
    g_TransformShaders[0].Create();

    g_TransformShaders[1].LoadVertexShader("transform.vs");
    g_TransformShaders[1].LoadFragmentShader("Orange.fs");
    g_TransformShaders[1].Create();

    g_TransformShaders[2].LoadVertexShader("transform.vs");
    g_TransformShaders[2].LoadFragmentShader("Orange.fs");
    g_TransformShaders[2].Create();

    
    glGenBuffers(totalFigure, vbos);
    glGenBuffers(totalFigure, ibos);
    glGenVertexArrays(totalFigure, vaos);

    constexpr size_t stride = sizeof(Vertex);

    uint32_t programs[totalFigure];
    programs[0] = g_TransformShaders[0].GetProgram();
    programs[1] = g_TransformShaders[1].GetProgram();
    programs[2] = g_TransformShaders[2].GetProgram();
    


    constexpr size_t strides = sizeof(DragonVertex);
    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(DragonVertices), DragonVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DragonIndices), DragonIndices, GL_STATIC_DRAW);

    int loc_position = glGetAttribLocation(programs[0], "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , false, strides, (void*)offsetof(DragonVertex, position));

    int loc_uv = glGetAttribLocation(programs[0], "a_texcoords");
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT
        , false, strides, (void*)offsetof(DragonVertex, uv));

    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    loadTexFromFile("dragon.png");
    

    //2eme objet
    glBindVertexArray(vaos[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, fig.size()*sizeof(Vertex), &fig[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice.size() * sizeof(int), &indice[0], GL_STATIC_DRAW);

    loc_position = glGetAttribLocation(programs[1], "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , false, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //3eme objet
    glBindVertexArray(vaos[2]);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, fig2.size() * sizeof(Vertex), &fig2[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_fig2.size() * sizeof(int), &indice_fig2[0], GL_STATIC_DRAW);

    loc_position = glGetAttribLocation(programs[1], "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , false, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    



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

    for (int i = 0; i < sizeof(g_TransformShaders) / sizeof(GLShader); i++) {
        g_TransformShaders[i].Destroy();
    }
}

void Render(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    float translationZ = -30.f;

    const float translations[totalFigure][16] = {
        {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, -30.f, 1.f
        },
        {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            10.f, 0.f,-30.f, 1.f
        },
        {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            -10.f, 0.f,-30.f, 1.f
        }
    };

    uint32_t programs[totalFigure];
    programs[0] = g_TransformShaders[0].GetProgram();
    programs[1] = g_TransformShaders[1].GetProgram();
    programs[2] = g_TransformShaders[2].GetProgram();

    figSizes[0] = _countof(DragonVertices);
    figSizes[1] = indice.size();
    figSizes[2] = indice_fig2.size();

    for (int i = 0; i < sizeof(programs) / sizeof(uint32_t); i++) {
        glUseProgram(programs[i]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);

        GLint textureLocation = glGetUniformLocation(programs[0], "u_sampler");
        glUniform1i(textureLocation, 0);

        GLint proj = glGetUniformLocation(programs[0], "u_projection");
        glUniformMatrix4fv(proj, 1, false, projection);

        GLint trans = glGetUniformLocation(programs[0], "u_translation");
        glUniformMatrix4fv(trans, 1, false, translations[i]);
        glBindVertexArray(vaos[i]);
        glDrawElements(GL_TRIANGLES, figSizes[i], GL_UNSIGNED_INT, 0);
    }

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