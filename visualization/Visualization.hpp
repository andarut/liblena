#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"
#include "Globals.hpp"

/* for text rendering */
#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

class Visualization {
public:
	static void error_cb(int err, const char* desc) {
		fprintf(stderr, "GLFW Error %d: %s\n", err, desc);
	}
	
	// static void key_cb(GLFWwindow* win, int key, int, int action, int) {
	// 	if (action != GLFW_PRESS) return;
	// 	if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL) {
	// 		scale++;
	// 	} else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) {
	// 		scale--;
	// 	}
	// }

	Visualization() {
		/* Init GLFW */
		glfwSetErrorCallback(error_cb);
		if (!glfwInit()) {
			printf("glfwInit failed\n");
		}

		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	}

	template <typename T>
	void show(const ImageChannel<T> &ch) {

		int scale = 100;

		printf("show %lld x %lld\n", ch.width(), ch.height());

		/* Init window */
		GLFWwindow* window = glfwCreateWindow(
			ch.width()*scale, ch.height()*scale, "RawImageData",
			nullptr, nullptr
		);
		if (!window) {
			glfwTerminate();
			printf("window creation failed\n");
			return;
		}
		glfwMakeContextCurrent(window);

		/* Register callbacks */
		// glfwSetKeyCallback(window, key_cb);
		// glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

		glPointSize(1);

		glViewport(0, 0, ch.width()*scale, ch.height()*scale);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, ch.width()*scale,  ch.height()*scale, 0,  -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		int px = 0;
		int py = 0;

		for (int i = 0; i < ch.height(); i++) {
			for (int j = 0; j < ch.width(); j++) {
				glColor3ub(ch(i,j), ch(i,j), ch(i,j));
				// printf("DRAW PIXEL %d %d\n", i, j);
				glBegin(GL_QUADS);
					glVertex2f(px,      py);
					glVertex2f(px+scale,   py);
					glVertex2f(px+scale, py+scale);
					glVertex2f(px,    py+scale);
				glEnd();
				px += scale;

			}
			py += scale;
			px=0;
		}


		glfwSwapBuffers(window);

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

    template <typename T>
	void show(const std::vector<ImageChannel<T>> &chs) {

        assert(chs.size() == 3);

		int scale = 50;

		printf("show %lld x %lld\n", chs[0].width(), chs[0].height());

		/* Init window */
		GLFWwindow* window = glfwCreateWindow(
			chs[0].width()*scale, chs[0].height()*scale, "RawImageData",
			nullptr, nullptr
		);
		if (!window) {
			glfwTerminate();
			printf("window creation failed\n");
			return;
		}
		glfwMakeContextCurrent(window);

		/* Register callbacks */
		// glfwSetKeyCallback(window, key_cb);
		// glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

		glPointSize(1);

		glViewport(0, 0, chs[0].width()*scale, chs[0].height()*scale);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, chs[0].width()*scale,  chs[0].height()*scale, 0,  -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		int px = 0;
		int py = 0;

		for (int i = 0; i < chs[0].height(); i++) {
			for (int j = 0; j < chs[0].width(); j++) {
				glColor3ub(chs[0](i,j), chs[1](i,j), chs[2](i,j));
				// printf("DRAW PIXEL %d %d\n", i, j);
				glBegin(GL_QUADS);
					glVertex2f(px,      py);
					glVertex2f(px+scale,   py);
					glVertex2f(px+scale, py+scale);
					glVertex2f(px,    py+scale);
				glEnd();
				px += scale;

			}
			py += scale;
			px=0;
		}


		glfwSwapBuffers(window);

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	~Visualization() {
		for (int i = 0; i < m_windows.size(); i++)
			glfwDestroyWindow(m_windows[i]);
		glfwTerminate();
	}
private:
    std::vector<GLFWwindow*> m_windows;
};

#endif // VISUALIZATION_H