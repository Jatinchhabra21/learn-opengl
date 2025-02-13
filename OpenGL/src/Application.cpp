#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<math.h>
#include<windows.h> 

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

const unsigned int VERTEX_COUNT = 120;

float x = 0.0f, y = 0.0f;

static float normalise_mouse_position(double pos)
{
    float m_pos = (float)((pos / 500)*2 - 1);
    return m_pos;
}

static void keyboard_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_A)
            x -= 0.01f;
        else if (key == GLFW_KEY_D)
            x += 0.01f;
        else if (key == GLFW_KEY_W)
            y += 0.01f;
        else if (key == GLFW_KEY_S)
            y -= 0.01f;
    }

    if (action == GLFW_RELEASE)
        std::cout << key << " is released now" << std::endl;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos = (double)((xpos / 500) * 2 - 1);
        ypos = (double)((ypos / 500) * 2 - 1);
        std::cout << "Cursor Position at (" << xpos << " , " << (ypos) << ")" << std::endl;
    }

}

static std::vector<float> GetPositions(float x, float y, float radius, unsigned int vertex_count)
{
    float theta = ((360.0 / (float)vertex_count)*3.142)/180.0;
    std::vector<float> positions = { x, y, (x + radius), y };

    for (unsigned int i = 1; i <= vertex_count; i++)
    {
        positions.push_back(x + (radius * cos(theta * i)));
        positions.push_back(y + (radius * sin(theta * i)));
    }
    positions.push_back((x + radius));
    positions.push_back(y);
    return positions;
}

static std::vector<unsigned int> GetIndices(unsigned int vertex_count)
{
    std::vector<unsigned int> indices = {};

    for (unsigned int i = 1, j = 2; i < vertex_count + 2 && j <= vertex_count + 2; i++, j++)
    {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(j);
    }
    
    return indices;
}
 
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(500, 500, "Circle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, keyboard_press_callback);

    GLenum err =  glewInit();

    if (err != GLEW_OK) {
        std::cout << "Error encountered" << std::endl;
    }

    std::cout << "[Debug] OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    float x = 0.0f, y = 0.0f, radius = 0.2f;
    
    {
        extern float x, y;

        std::vector<float> positions = GetPositions(x, y, radius, VERTEX_COUNT);

        std::vector<unsigned int> indices = GetIndices(VERTEX_COUNT);

        VertexArray va;
        VertexBuffer vb(&positions[0], positions.capacity() * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(&indices[0], indices.size());

        Shader shader("res/Shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniform1f("u_Offset", 0.2);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float r,g,b;
        r = g = b = 0.0f;
        float increment = 0.05f;

        float theta = 0.1f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, g, b, 1.0f);
            shader.SetUniform2f("u_Offset", x, y);

            va.Bind();
            ib.Bind();

            glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, 0);
            va.Unbind();

            if (r > 1.0f)
                g += increment;
            if (g > 1.0f)
                b += increment;
            if (b > 1.0f)
            {
                r = 0.0f;
                b = 0.0f;
                g = 0.0f;
            }
            else
                r += increment;

            if (theta > 6.28f)
                theta = 0.1f;
            else
                theta += 0.1f;

            // std::cout << r << "," << g << "," << b<<std::endl;

            /*x = cos(theta) * 0.75f;
            y = sin(theta) * 0.75f;*/


            Sleep(50);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}