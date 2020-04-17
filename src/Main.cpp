#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow *pWindow, int width, int height);
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);


int main(int argc, char argv[])
{
    if (!glfwInit())
    {
        std::cout << "ERROR: gldwInnit() failed!" << std::endl;
        system("pause");
        return -1;
    }

    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "BattleCity", nullptr, nullptr);

    if (!pWindow)
    {
        std::cout << "ERROR: No't create GLFWwindow!" << std::endl;
        system("pause");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); //callback
    glfwSetKeyCallback(pWindow, glfwKeyCallback);



    glfwMakeContextCurrent(pWindow);
	
	if(!gladLoadGL())
	{
		std::cout << "ERROR: Can't load GLAD!" << std::endl;
        system("pause");
		return -1;
	}
	

    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	glClearColor(0, 1, 0, 1);

   //------------------------------------------------------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(pWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(pWindow);

        glfwPollEvents();
    }
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    glfwTerminate();
	return 0;
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}
