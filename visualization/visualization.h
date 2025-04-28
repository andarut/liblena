#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "../include/lena.h"

/* for text rendering */
#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

struct Visualization {

	std::vector<GLFWwindow*> windows;

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
	void show(RawImageData<T> &data) {

		int scale = 100;

		/* Init window */
		GLFWwindow* window = glfwCreateWindow(
			data.width*scale, data.height*scale, "RawImageData",
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

		RawChannelData<T> *R = &data[0];
		RawChannelData<T> *G = &data[1];
		RawChannelData<T> *B = &data[2];

		glViewport(0, 0, data.width*scale, data.height*scale);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, data.width*scale,  data.height*scale, 0,  -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		int px = 0;
		int py = 0;

		

		for (int i = 0; i < data.height; i++) {
			for (int j = 0; j < data.width; j++) {
				glColor3ub((*R)(i,j), (*G)(i,j), (*B)(i,j));
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
	void show(const RawChannelData<T> &data) {

		int scale = 100;

		printf("show %lld x %lld\n", data.width, data.height);

		/* Init window */
		GLFWwindow* window = glfwCreateWindow(
			data.width*scale, data.height*scale, "RawImageData",
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

		glViewport(0, 0, data.width*scale, data.height*scale);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, data.width*scale,  data.height*scale, 0,  -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		int px = 0;
		int py = 0;

		for (int i = 0; i < data.height; i++) {
			for (int j = 0; j < data.width; j++) {
				glColor3ub(data(i,j), data(i,j), data(i,j));
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
	void show(const SubsampledChannelData<T> &data) {

		int scale = 100;

		/* Init window */
		GLFWwindow* window = glfwCreateWindow(
			data.width*scale, data.height*scale, "RawImageData",
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

		glViewport(0, 0, data.width*scale, data.height*scale);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, data.width*scale,  data.height*scale, 0,  -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);

		int px = 0;
		int py = 0;

		for (int i = 0; i < data.height; i++) {
			for (int j = 0; j < data.width; j++) {
				glColor3ub(data(i,j), data(i,j), data(i,j));
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
		for (int i = 0; i < windows.size(); i++)
			glfwDestroyWindow(windows[i]);
		glfwTerminate();
	}
};

Visualization g_visualization;

int main() {
	// std::ifstream ppm_file("/Users/andarut/dev/liblena/tests/tests_data/ppm_image.ppm");
	// std::ifstream ppm_file("/Users/andarut/dev/liblena/tests/tests_data/ppm_image_4K.ppm");
	// std::ifstream ppm_file("/Users/andarut/dev/liblena/tests/tests_data/ppm_image_raw.ppm");
	// std::ifstream ppm_file("/Users/andarut/dev/liblena/tests/tests_data/4x2.ppm");
	std::ifstream ppm_file("/Users/andarut/dev/liblena/tests/tests_data/8x4.ppm");

	auto ppm_image = read_ppm_image<u8>(ppm_file);
	auto ycbcr_data = rgb_to_ycbcr(ppm_image);
	// auto ycbcr_data = ppm_image;

	auto mode1 = SubsamplingMode(4, 4, 4);
	auto mode2 = SubsamplingMode(8, 4, 4);

	auto subsampled_data1 = encode_subsampling(ycbcr_data, mode1);
	auto subsampled_data2 = encode_subsampling(ycbcr_data, mode2);

	auto decoded_data1 = decode_subsampling(subsampled_data1);
	auto decoded_data2 = decode_subsampling(subsampled_data2);

	auto decoded_rgb_data1 = ycbcr_to_rgb(decoded_data1);
	auto decoded_rgb_data2 = ycbcr_to_rgb(decoded_data2);

	// auto decoded_rgb_data1 = decoded_data1;
	// auto decoded_rgb_data2 = decoded_data2;

	printf("=== MODE 1 ===\n");
	printf("original data size = %lld\n", ppm_image.size());
	printf("compressed data size = %lld\n", subsampled_data1.size());
	printf("decoded data size = %lld\n", decoded_rgb_data1.size());
	printf("PNSR = %f\n", PSNR(ppm_image, decoded_rgb_data1));
	printf("==============\n");
	
	printf("=== MODE 2 ===\n");
	printf("original data size = %lld\n", ppm_image.size());
	printf("compressed data size = %lld\n", subsampled_data2.size());
	printf("decoded data size = %lld\n", decoded_rgb_data2.size());
	printf("PNSR = %f\n", PSNR(ppm_image, decoded_rgb_data2));
	printf("==============\n");
	
	auto test_data = SubsampledChannelData<u8>(16, SubsamplingMode(8, 4, 4));
	
	test_data.original_width = 8;
	test_data.original_height = 4;

	test_data.data = {
		255, 255, 0, 0,
		0, 255, 0, 255,
		0, 0, 255 ,255,
		255, 0, 255, 0,
	};

	auto test_decoded = decode_subsampling(test_data);

	g_visualization.show(test_decoded);

	return 0;
}