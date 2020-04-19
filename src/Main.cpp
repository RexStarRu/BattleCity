#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

                                                          //������ ������ (3)

GLfloat point[] = {
    0.0f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

                                                           //������ ������ (3)

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                       

int g_windowSizeX = 640;    //���������� ������� ���� (������)
int g_windowSizeY = 480;    //���������� ������� ���� (������)

void glfwWindowSizeCallback(GLFWwindow *pWindow, int width, int height);                    //callback �� ��������� �������� ���� (��������)
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);     //callback �� ������� ������� (��������)


int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        std::cout << "ERROR: gldwInnit() failed!" << std::endl;
        system("pause");
        return -1;
    }

    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                      //����� ����������� ������ openGL (major)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);                      //����� ����������� ������ openGL (minor)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      //����� ������� openGL -> CORE (�.�. ��� ����������� ���� � �� ��������)



    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "BattleCity", nullptr, nullptr); //�������� ����

    if (!pWindow)
    {
        std::cout << "ERROR: No't create GLFWwindow!" << std::endl;
        system("pause");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); //������� ���� callback �� ��������� �������� ����
    glfwSetKeyCallback(pWindow, glfwKeyCallback);               //������� ���� callback �� ������� �������



    glfwMakeContextCurrent(pWindow);  //������ ���� ������� ����������
	
	if(!gladLoadGL())
	{
		std::cout << "ERROR: Can't load GLAD!" << std::endl;
        system("pause");
		return -1;
	}
	

    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;           //����� ���������� � ����������
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;    //����� ���������� � OpenGL � � ���������
	
	glClearColor(0.0, 0.5, 0.5, 1);

    //-----------------------------------------------------------------------------------------------------------------------------------------------
    {
        Resorce::ResorceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");

        if (!pDefaultShaderProgram)
        {
            std::cerr << "ERROR: Can't create shader program: DefaultShader!" << std::endl;
            return -1;
        }

        GLuint points_vbo = 0;                      //������ vbo 
        glGenBuffers(1, &points_vbo);               //���������� 1 ������ vbo ��� ���������
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); //������ ������ vbo �������

        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); //������� ������ � ���������� (������� ����������� ��� �������� �������, �.�. ��� vbo)


        GLuint colors_vbo = 0;                      //����� vbo 
        glGenBuffers(1, &colors_vbo);               //���������� 1 ������ vbo ��� colors
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); //������ ������ vbo �������

        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); //������� ������ � ���������� (������� ����������� ��� �������� �������, �.�. ��� vbo)


        GLuint vao = 0;                     //����� vertexArraysObject
        glGenVertexArrays(1, &vao);         //���������� 1 ���������� �����
        glBindVertexArray(vao);             //������ ��� �������


        glEnableVertexAttribArray(0);                                   //�������� ������� �������� � ������� (layout(location = 0))
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                      //����� ������ ������� ������ points_vbo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    //��������� ������ (������� �������, ���-�� ��������� (3 �� vec3), ��� ������, ����������� �� ������ (false), ��� ��������, �������� �� ������ �������)

        glEnableVertexAttribArray(1);                                   //�������� ������� �������� � ������� (layout(location = 1))
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);                      //����� ������ ������� ������ colors_vbo
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    //��������� ������



       //-----------------------------------------------------------�������� ����--------------------------------------------------------------------------------------------------
        while (!glfwWindowShouldClose(pWindow))
        {
            glClear(GL_COLOR_BUFFER_BIT);


            pDefaultShaderProgram->use();     //���������� ������ ������� ���������� ����������
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3); //������������ ����������� � 1� (0�) ����� 3 �����


            glfwSwapBuffers(pWindow);

            glfwPollEvents();
        }
        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    }
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    glfwTerminate();
	return 0;
}

//==============================================================================================================================================================================

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
