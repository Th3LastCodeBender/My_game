#include "header.hpp"

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "My_Game", NULL, NULL);
	glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);opengl c++ tutorial
        
		glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwTerminate();
	return(0);
}