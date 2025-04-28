#include "visualization.h"

// int   imgWidth   = 16;
// int   imgHeight  = 16;
// float scale      = 24.0f;
// float textThresh = 8.0f;

// std::vector<unsigned char> pixels;

// // GLFW error callback
// void error_cb(int err, const char* desc) {
// 	fprintf(stderr, "GLFW Error %d: %s\n", err, desc);
// }

// // Handle key presses for zooming
// void key_cb(GLFWwindow* win, int key, int, int action, int) {
// 	if (action != GLFW_PRESS) return;
// 	if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL) {
// 		scale += 2.0f;
// 	} else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) {
// 		scale = std::max(4.0f, scale - 2.0f);
// 	}
// }

// // Resize viewport and projection on window size change
// void framebuffer_size_cb(GLFWwindow* win, int w, int h) {
	// glViewport(0, 0, w, h);
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	// // Ortho: [0, imgWidth*scale] × [0, imgHeight*scale]
	// glOrtho(0, imgWidth * scale, 0, imgHeight * scale, -1, 1);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
// }

// int main() {
// 	// 1) Generate a sample gradient image
// 	pixels.resize(imgWidth * imgHeight * 3);
// 	for (int y = 0; y < imgHeight; ++y) {
// 		for (int x = 0; x < imgWidth; ++x) {
// 			int i = (y * imgWidth + x) * 3;
// 			pixels[i+0] = static_cast<unsigned char>(x * 255 / (imgWidth-1));
// 			pixels[i+1] = static_cast<unsigned char>(y * 255 / (imgHeight-1));
// 			pixels[i+2] = 128;
// 		}
// 	}

// 	// 2) Initialize GLFW
// 	glfwSetErrorCallback(error_cb);
// 	if (!glfwInit()) return -1;
// 	GLFWwindow* window = glfwCreateWindow(
// 		imgWidth * (int)scale, imgHeight * (int)scale,
// 		"Image with RGB Labels (GLFW)", nullptr, nullptr
// 	);
// 	if (!window) {
// 		glfwTerminate();
// 		return -1;
// 	}
// 	glfwMakeContextCurrent(window);

// 	// 3) Register callbacks
// 	glfwSetKeyCallback(window, key_cb);
// 	glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
// 	// Set up initial projection
// 	framebuffer_size_cb(window, imgWidth * (int)scale, imgHeight * (int)scale);

// 	// 4) Render loop
// 	while (!glfwWindowShouldClose(window)) {
// 		glClear(GL_COLOR_BUFFER_BIT);

// 		bool showText = (scale >= textThresh);
// 		for (int y = 0; y < imgHeight; ++y) {
// 			for (int x = 0; x < imgWidth; ++x) {
// 				int idx = (y * imgWidth + x) * 3;
// 				unsigned char r = pixels[idx];
// 				unsigned char g = pixels[idx+1];
// 				unsigned char b = pixels[idx+2];

// 				float fx = x * scale;
// 				float fy = y * scale;

// 				// Draw pixel quad
// 				glColor3ub(r, g, b);
// 				glBegin(GL_QUADS);
// 				  glVertex2f(fx,         fy);
// 				  glVertex2f(fx + scale, fy);
// 				  glVertex2f(fx + scale, fy + scale);
// 				  glVertex2f(fx,         fy + scale);
// 				glEnd();

// 				// Draw RGB text
// 				if (showText) {
// 					int lum = (int(r) + int(g) + int(b)) / 3;
// 					if (lum > 128) glColor3f(0,0,0);
// 					else           glColor3f(1,1,1);

// 					std::ostringstream ss;
// 					ss << "(" << int(r) << "," << int(g) << "," << int(b) << ")";
// 					char buffer[256];
// 					int quads = stb_easy_font_print(
// 						fx + 2.0f, fy + scale - 14.0f,
// 						(char*)ss.str().c_str(), nullptr, buffer, sizeof(buffer)
// 					);
// 					glEnableClientState(GL_VERTEX_ARRAY);
// 					glVertexPointer(2, GL_FLOAT, 16, buffer);
// 					glDrawArrays(GL_QUADS, 0, quads * 4);
// 					glDisableClientState(GL_VERTEX_ARRAY);
// 				}
// 			}
// 		}

// 		glfwSwapBuffers(window);
// 		glfwPollEvents();   // explicit loop control :contentReference[oaicite:3]{index=3}
// 	}

// 	// 5) Cleanup
// 	glfwDestroyWindow(window);
// 	glfwTerminate();
// 	return 0;
// }
