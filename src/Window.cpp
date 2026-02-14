#include "Window.h"
#include <GLFW/glfw3.h>

Window::~Window() {
    free();
}

int Window::init(size_t width, size_t height, const std::string& title) {
    m_width = width;
    m_height = height;
    m_title = title;
    m_state = WindowState::INITIALIZED;

    if (!glfwInit()) {
        ERROR("failed to initialize GLFW\n");
        return 1;
    }
    INFO("GLFW initialized successfully\n");

    glfwSetErrorCallback([](int error, const char* description){
        ERROR("GLFW Error %d: %s\n", error, description);
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on macOS
#endif

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_window) {
        ERROR("failed to create GLFW window\n");
        return 1;
    }

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable V-Sync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR("failed to initialize GLAD\n");
        return 1;
    }

    return 0;
}

void Window::free() {
    glfwDestroyWindow(m_window);
    m_window = nullptr;

    m_state = WindowState::DESTROYED;
}

int Window::render() {
    glfwPollEvents();

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    glViewport(0, 0, w, h);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return 0;
}