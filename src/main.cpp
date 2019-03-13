#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <stdio.h>
#include "Application.h"
#include "freeimage.h"

void PrintOpenGLVersion();

int main () {
    FreeImage_Initialise();
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    const int WindowWidth = 800;
    const int WindowHeight = 600;
    
    GLFWwindow* window = glfwCreateWindow (WindowWidth, WindowHeight, "Computergrafik - Hochschule Osnabrück", NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: can not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glEnable(GL_LINE_SMOOTH);

#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif

    PrintOpenGLVersion();
    
    {
        double lastTime=0;
		bool allKeysReleased = true;

        Application App(window);
        App.start();
        while (!glfwWindowShouldClose (window)) {
            double now = glfwGetTime();
            double delta = now - lastTime;
            lastTime = now;

            // once per frame
            glfwPollEvents();
            App.update((float)delta, allKeysReleased);
            App.draw();
            glfwSwapBuffers (window);

			// All keys must be released 
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && 
				glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && 
				glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && 
				glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE &&
				glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE &&
				glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
				allKeysReleased = true;
			}
			else {
				allKeysReleased = false;
			}
        }
        App.end();
    }
    
    glfwTerminate();
    return 0;
}


void PrintOpenGLVersion()
{
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
}
