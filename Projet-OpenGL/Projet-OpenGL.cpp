#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "libs/common/GLShader.h"

#include "Vertex.h"
#include "DragonData.h"

// attention, ce define ne doit etre specifie que dans 1 seul fichier cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

GLShader g_TransformShader;

GLuint VBO;
GLuint IBO;
GLuint VAO;

GLuint TexID;



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

    //On active le test de profondeur et le face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(DragonVertices), DragonVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DragonIndices), DragonIndices, GL_STATIC_DRAW);

    const size_t stride = sizeof(DragonVertex);

    auto program = g_TransformShader.GetProgram();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

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

    loadTexFromFile("dragon.png");

    return true;
}

void Terminate()
{
    glDeleteTextures(1, &TexID);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
    const float zFar = 100.0f;
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

    GLint trans = glGetUniformLocation(program, "u_translation");
    glUniformMatrix4fv(trans, 1, false, translation);

    glBindVertexArray(VAO);

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