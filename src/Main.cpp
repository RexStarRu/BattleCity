#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

                                                          //массив вершин (3)

GLfloat point[] = {
    0.0f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

                                                           //массив цветов (3)

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
                                                       

int g_windowSizeX = 640;    //глобальные размеры окна (ширина)
int g_windowSizeY = 480;    //глобальные размеры окна (высота)

void glfwWindowSizeCallback(GLFWwindow *pWindow, int width, int height);                    //callback на изменение размеров окна (прототип)
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode);     //callback на нажатие клавишь (прототип)


int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        std::cout << "ERROR: gldwInnit() failed!" << std::endl;
        system("pause");
        return -1;
    }

    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                      //задаём минимальную версию openGL (major)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);                      //задаём минимальную версию openGL (minor)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      //задаём профиль openGL -> CORE (т.е. без стандартных окон и их настроек)



    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "BattleCity", nullptr, nullptr); //создание окна

    if (!pWindow)
    {
        std::cout << "ERROR: No't create GLFWwindow!" << std::endl;
        system("pause");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); //передаём свой callback на изменение размеров окна
    glfwSetKeyCallback(pWindow, glfwKeyCallback);               //передаём свой callback на нажатие клавишь



    glfwMakeContextCurrent(pWindow);  //делаем окно текущим контекстом
	
	if(!gladLoadGL())
	{
		std::cout << "ERROR: Can't load GLAD!" << std::endl;
        system("pause");
		return -1;
	}
	

    std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;           //выдаёт информацию о видеокарте
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;    //выдаёт информацию о OpenGL и её драйверах
	
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

        GLuint points_vbo = 0;                      //хендер vbo 
        glGenBuffers(1, &points_vbo);               //генерируем 1 буффер vbo для вертексов
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); //делаем буффер vbo текущем

        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); //передаём данные в видеокарту (команда выполняется для текущего буффера, т.е. для vbo)


        GLuint colors_vbo = 0;                      //хендл vbo 
        glGenBuffers(1, &colors_vbo);               //генерируем 1 буффер vbo для colors
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); //делаем буффер vbo текущем

        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); //передаём данные в видеокарту (команда выполняется для текущего буффера, т.е. для vbo)


        GLuint vao = 0;                     //хендл vertexArraysObject
        glGenVertexArrays(1, &vao);         //генерируем 1 вертексный эррэй
        glBindVertexArray(vao);             //делаем его текущим


        glEnableVertexAttribArray(0);                                   //включаем позиции заданные в шейдере (layout(location = 0))
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                      //снова делаем текущем буффер points_vbo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    //Связываем данные (нулевая позиция, кол-во элементов (3 по vec3), тип данных, нормировать ли данные (false), шаг смещения, смещение от начала массива)

        glEnableVertexAttribArray(1);                                   //включаем позиции заданные в шейдере (layout(location = 1))
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);                      //снова делаем текущем буффер colors_vbo
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    //Связываем данные



       //-----------------------------------------------------------Основной цикл--------------------------------------------------------------------------------------------------
        while (!glfwWindowShouldClose(pWindow))
        {
            glClear(GL_COLOR_BUFFER_BIT);


            pDefaultShaderProgram->use();     //подключаем шейдер который необходимо отрисовать
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3); //отрисовываем триугольник с 1й (0й) точки 3 грани


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
