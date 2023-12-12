#include "Test2Quads.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace test {

float vertices[] = {
    // ---- Pos ---- // ---- Tex Coords ---- //
    000.0f, 100.0f,     0.0f, 0.0f, //bottom left
    100.0f, 100.0f,     1.0f, 0.0f,  //bottom right
    100.0f, 000.0f,     1.0f, 1.0f,  //top right
    000.0f, 000.0f,     0.0f, 1.0f  //top left
};

unsigned int indices[] = {
    0,1,2,
    2,3,0
};

Test2Quads::Test2Quads() : ib(indices, 6)
{
    VertexBuffer vb = VertexBuffer(vertices, 4*4*sizeof(GLfloat));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    shader.Bind();
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
}

Test2Quads::~Test2Quads()
{

}

void Test2Quads::OnUpdate(float deltaTime, GLFWwindow *window)
{
    float projectionWidth = 1920.0f;
    glfwGetWindowSize(window, &width, &height);
    GLCall(glViewport(0, 0, width, height));
    float windowAspect = ((float)width/(float)height);
    float projectionHeight = projectionWidth/windowAspect;
    float left = -projectionWidth/2.0f;
    float right = projectionWidth/2.0f;
    float top = -projectionHeight/2.0f;
    float bottom = projectionHeight/2.0f;
    proj = glm::ortho(left, right, bottom, top, 0.0001f, 10000.0f);
    model = glm::mat4(1.0f);

    // keyboard input;
    int state = glfwGetKey(window, GLFW_KEY_W);
    if (state == GLFW_PRESS)
    {
        camera.moveCamera(0, -10, 0);
    }
    state = glfwGetKey(window, GLFW_KEY_S);
    if (state == GLFW_PRESS)
    {
        camera.moveCamera(0, 10, 0);
    }
    state = glfwGetKey(window, GLFW_KEY_A);
    if (state == GLFW_PRESS)
    {
        camera.moveCamera(-10, 0, 0);
    }
    state = glfwGetKey(window, GLFW_KEY_D);
    if (state == GLFW_PRESS)
    {
        camera.moveCamera(10, 0, 0);
    }
}

void Test2Quads::OnRender(Renderer renderer)
{
    {
        glm::mat4 view = camera.GetPosition();
        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
    }
    {
        glm::mat4 view = camera.GetPosition();
        model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 0.0f));
        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
    }

}

}
