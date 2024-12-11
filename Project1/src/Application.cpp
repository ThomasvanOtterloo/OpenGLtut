#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] = {
			-0.5f,-0.5f, 0.0f, 0.0f, // 0
			 0.5f,-0.5f, 1.0f, 0.0f, // 1
			 0.5f, 0.5f, 1.0f, 1.0f, // 2
			-0.5f, 0.5f, 0.0f, 1.0f // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		VertexArray va; // vertex array object
		VertexBuffer vb(positions, 4 * 4 * sizeof(float)); // contains the size of the positions array
		VertexBufferLayout layout; // layout of the vertex buffer
		layout.Push<float>(2); // 2 floats for the position
		layout.Push<float>(2); // 2 floats for the position
		va.AddBuffer(vb, layout); // adds the vertex buffer to the vertex array


		
		glEnableVertexAttribArray(0); // enables the vertex attribute array
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); // links the vertex buffer to the vertex array

		IndexBuffer ib(indices, 6); // does the same thing as the vertex buffer but for the index buffer




		// shaders that are used to draw the triangle
	
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


		Texture texture("res/textures/emotions.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);




		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;


		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();
			ib.Bind();

			renderer.Draw(va, ib, shader);
			
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
    glfwTerminate();
    return 0;
}