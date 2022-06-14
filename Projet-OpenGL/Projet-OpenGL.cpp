// seulement si glew32s.lib
//#define GLEW_STATIC 1
#include "GL/glew.h"

#include <GLFW/glfw3.h>

#include "common/GLShader.h"

#include "Vertex.h"

// attention, ce define ne doit etre specifie que dans 1 seul fichier cpp
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb/stb_image.h"

GLShader g_TextureShader;

GLuint VBO;
GLuint VAO;

GLuint TexID;

bool Initialise()
{
    GLenum ret = glewInit();

    g_TextureShader.LoadVertexShader("texture.vs");
    g_TextureShader.LoadFragmentShader("texture.fs");
    g_TextureShader.Create();

    const Vertex triangle[] = {
    {{-0.5f, -0.5f}, {0.f, 0.f}, {255, 0, 0, 255}},   // sommet 0
    {{0.5f, -0.5f},  {1.f, 0.f}, {0, 255, 0, 255}},   // sommet 1
    {{0.0f, 0.5f},   {0.f, 1.f}, {0, 0, 255, 255}}    // sommet 2
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData alloue et transfert 3 Vertex issus du tableau triangle[]
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(Vertex) * 3, triangle, GL_STATIC_DRAW);
    // je recommande de reinitialiser les etats a la fin pour eviter les effets de bord

    constexpr size_t stride = sizeof(Vertex);// sizeof(float) * 5;

    // 
    auto program = g_TextureShader.GetProgram();

    // VAO ---
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 0 = adresse memoire systeme, sinon GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int loc_position = glGetAttribLocation(program, "a_position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 2, GL_FLOAT
        , false, stride, (void*)offsetof(Vertex, position));

    int loc_uv = glGetAttribLocation(program, "a_texcoords");
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT
        , false, stride, (void*)offsetof(Vertex, uv));

    int loc_color = glGetAttribLocation(program, "a_color");
    glEnableVertexAttribArray(loc_color);
    glVertexAttribPointer(loc_color, 4, GL_UNSIGNED_BYTE
        , true, stride, (void*)offsetof(Vertex, color));

    // La bonne pratique est de reinit a zero
    // MAIS ATTENTION, toujours le VAO en premier
    // sinon le VAO risque d'enregistrer les modifications
    // de VertexAttrib et VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenTextures(1, &TexID);
    glBindTexture(GL_TEXTURE_2D, TexID);
    int w, h;
    uint8_t* data = stbi_load("dragon.png", &w, &h, nullptr, STBI_rgb_alpha);
    if (data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8
            , w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    // filtre bilineaire
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // filtre trilineaire (necessite mipmap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

void Terminate()
{
    glDeleteTextures(1, &TexID);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    g_TextureShader.Destroy();
}

void Render(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // etape a. A vous de recuperer/passer les variables width/height
    glViewport(0, 0, width, height);
    // etape b. Notez que glClearColor est un etat, donc persistant
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    // etape c. on specifie le shader program a utiliser
    auto program = g_TextureShader.GetProgram();
    glUseProgram(program);
    // etape d. 

    //static const float color[] = {
    //    1.0f, 0.0f, 0.0f,   // sommet 0
    //    0.0f, 1.0f, 0.0f,   // sommet 1
    //    0.0f, 0.0f, 1.0f    // sommet 2
    //};

    // etape e. 
    GLint timeLocation = glGetUniformLocation(program, "u_time");
    const float time = (float) glfwGetTime();
    glUniform1f(timeLocation, time);

    // 
    // Rappel sur l'arithmetique des pointeurs : 
    // char * adresse = ...;
    // float * pointeur = (float *)adresse;
    // float * p2 = pointeur + 2;
    // la ligne precedente est similaire a
    // float * p2 = adresse + sizeof(float)*2;
    // 
    // etape f. dessin de triangles dont la definition provient d’un tableau
    // le rendu s’effectue ici en prenant 3 sommets a partir du debut du tableau (0)

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TexID);

    GLint textureLocation = glGetUniformLocation(program, "u_sampler");
    // 0 ici correspond au canal 0 cad GL_TEXTURE0
    glUniform1i(textureLocation, 0);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // on suppose que la phase d’echange des buffers front et back
    // le « swap buffers » est effectuee juste apres

}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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