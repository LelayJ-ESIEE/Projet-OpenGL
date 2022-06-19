#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "libs/common/GLShader.h"

#include "Vertex.h"
#include "DragonData.h"
#include "Vec.h"
#include "Matrice4D.h"
#include "Camera.h"

#include <iostream>
#include <iostream>

// attention, ce define ne doit etre specifie que dans 1 seul fichier cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLShader g_TransformShader;

GLuint VBO;
GLuint VBO2;
GLuint IBO2;

GLuint IBO;
GLuint VAO;

GLuint TexID;
GLuint vbos[2];
GLuint ibos[2];
Camera camera;

vec3 cameraPos = { 0.0f, 0.0f, 3.0f };
vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 cameraUp = { 0.0f, 1.0f, 0.0f };


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
    const float triangle[] = {
      -1.0, -1.0, 0.0,
      1.0, -1.0, 0.0,
      0.0, 1.0, 0.0,
    };
    const int indices[] = {
        0,2,1,2,1,0
    };
    GLenum ret = glewInit();

    g_TransformShader.LoadVertexShader("transform.vs");
    g_TransformShader.LoadFragmentShader("transform.fs");
    g_TransformShader.Create();

    //On active le test de profondeur et le face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    /*glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(triangle), triangle, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3, indices, GL_STATIC_DRAW);*/



    const size_t stride = sizeof(DragonVertex);

    auto program = g_TransformShader.GetProgram();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(2, ibos);
    glGenBuffers(2, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(DragonVertices), DragonVertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DragonIndices), DragonIndices, GL_STATIC_DRAW);

    int loc_position = glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT
        , false, stride, (void*)offsetof(DragonVertex, position));

    int loc_uv = glGetAttribLocation(program, "a_texcoords");
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT
        , false, stride, (void*)offsetof(DragonVertex, uv));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(triangle), triangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    loadTexFromFile("dragon.png");

    return true;
}

void Terminate()
{
    glDeleteTextures(1, &TexID);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, vbos);

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

    const float near = 0.1f;
    const float far = 100;
    const float aspect = float(width) / float(height);
    const float fov = 45.0f * M_PI / 180.0f;
    const float f = 1.0f / tanf(fov / 2.0f);
    float* projection = Matrice4D::perspective(f, aspect, far, near);

    GLint proj = glGetUniformLocation(program, "u_projection");
    glUniformMatrix4fv(proj, 1, false, projection);
    float translationX = 0.f;
    float translationY = 0.f;
    float translationZ = -30.f;


    float* translation = Matrice4D::translation(translationX, translationY, translationZ);

    //camera.position = { 0.0f, 0.0f, 0.0f, 1.0f };
    //camera.worldMatrix = translation;
    //camera.viewMatrix = Matrice4D::LookAt(Vec::vec4ToVec3(camera.WorldToView()), { 0.5f,0.5f,0.5f }, { 1.0f,1.0f,1.0f });
    //camera.projectionMatrix = projection;
    float* view = Matrice4D::LookAt(cameraPos, Vec::add(cameraPos, cameraFront), cameraUp);

    GLint trans = glGetUniformLocation(program, "u_translation");
    glUniformMatrix4fv(trans, 1, false, translation);

    glBindVertexArray(VAO);
    for (int i = 0; i < 3; i++) {
        glDrawElements(GL_TRIANGLES, _countof(DragonVertices), GL_UNSIGNED_SHORT, 0);
    }


}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos = Vec::add(cameraPos, Vec::multiplyByFloat(cameraFront, cameraSpeed));
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

        processInput(window);
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