#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>

#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "libs/common/GLShader.h"

#include "Vertex.h"
#include "DragonData.h"
#include "Vec.h"
#include "Matrice4D.h"
#include "Camera.h"


#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>

#include "Header.h"
#define totalFigure 3



// Ne définir qu'une fois
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLShader g_Shaders[totalFigure];

Camera camera;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f );
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f );

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int figSizes[totalFigure];

GLuint TexID;
GLuint vbos[totalFigure];
GLuint ibos[totalFigure];
GLuint vaos[totalFigure];
std::vector<int> indice;
std::vector<Vertex> fig = loadObj("cube.obj", indice);

std::vector<int> indice_fig2;
std::vector<Vertex> fig2 = loadObj("leaftree.obj", indice_fig2);

void loadTexture(const char* filename) {

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

    g_Shaders[0].LoadVertexShader("transform.vs");
    g_Shaders[0].LoadFragmentShader("transform.fs");
    g_Shaders[0].Create();

    g_Shaders[1].LoadVertexShader("transform.vs");
    g_Shaders[1].LoadFragmentShader("Orange.fs");
    g_Shaders[1].Create();

    g_Shaders[2].LoadVertexShader("transform.vs");
    g_Shaders[2].LoadFragmentShader("Orange.fs");
    g_Shaders[2].Create();
    
    glGenBuffers(totalFigure, vbos);
    glGenBuffers(totalFigure, ibos);
    glGenVertexArrays(totalFigure, vaos);

    uint32_t programs[totalFigure];
    programs[0] = g_Shaders[0].GetProgram();
    programs[1] = g_Shaders[1].GetProgram();
    programs[2] = g_Shaders[2].GetProgram();
    


    constexpr size_t stride_dragon = sizeof(DragonVertex);
    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(DragonVertices), DragonVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DragonIndices), DragonIndices, GL_STATIC_DRAW);

    int loc_position = glGetAttribLocation(programs[0], "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , false, stride_dragon, (void*)offsetof(DragonVertex, position));

    int loc_uv = glGetAttribLocation(programs[0], "a_texcoords");
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT
        , false, stride_dragon, (void*)offsetof(DragonVertex, uv));

    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    loadTexture("dragon.png");
    

    //2eme objet

    constexpr size_t stride_fig = sizeof(Vertex);
    glBindVertexArray(vaos[1]);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, fig.size() * sizeof(Vertex), &fig[0], GL_STATIC_DRAW);

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
        , false, stride_fig, (void*)offsetof(Vertex, position));

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

    for (int i = 0; i < sizeof(g_Shaders) / sizeof(GLShader); i++) {
        g_Shaders[i].Destroy();
    }
}

void Render(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float near = 0.1f;
    const float far = 100;

    const float aspect = float(width) / float(height);
    const float fov = 45.0f * M_PI / 180.0f;
    const float f = 1.0f / tanf(fov / 2.0f);
    const float projection[] = {
        f / aspect, 0.f, 0.f, 0.f,
        0.f, f, 0.f, 0.f,
        0.f, 0.f, ((far + near) / (near - far)), -1.f,
        0.f, 0.f, ((2 * near * far) / (near - far)), 0.f
    };

    const float models[totalFigure][16] = {
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
    programs[0] = g_Shaders[0].GetProgram();
    programs[1] = g_Shaders[1].GetProgram();
    programs[2] = g_Shaders[2].GetProgram();

    figSizes[0] = _countof(DragonVertices);
    figSizes[1] = indice.size();
    figSizes[2] = indice_fig2.size();

    for (int i = 0; i < sizeof(programs) / sizeof(uint32_t); i++) {
        glUseProgram(programs[i]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        GLint viewCam = glGetUniformLocation(programs[i], "v_view");
        glUniformMatrix4fv(viewCam, 1, false, glm::value_ptr(view));

        GLint textureLocation = glGetUniformLocation(programs[i], "u_sampler");
        glUniform1i(textureLocation, 0);

        GLint proj = glGetUniformLocation(programs[i], "u_projection");
        glUniformMatrix4fv(proj, 1, false, projection);

        GLint trans = glGetUniformLocation(programs[i], "u_model");
        glUniformMatrix4fv(trans, 1, false, models[i]);
        glBindVertexArray(vaos[i]);
        glDrawElements(GL_TRIANGLES, figSizes[i], GL_UNSIGNED_INT, 0);
    }
}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

        processInput(window);
    }

    Terminate();

    glfwTerminate();
    return 0;
}