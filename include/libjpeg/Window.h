#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Utils.hpp"
#include "Logger.hpp"

enum class WindowState {
    CREATED,
    INITIALIZED,
    DESTROYED
};

/*

RAII-class for managing the opengl window

*/
class Window {
public:
    ~Window();

    int init(size_t width=800, size_t height=600, const std::string& title="Window");

    virtual int render();

    bool shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }
    std::pair<int, int> renderSize() {
        int w = 4, h = 2;
        glfwGetFramebufferSize(m_window, &w, &h);
        return {w, h};
    }
    GLFWwindow* getGLFWWindow() const {
        return m_window;
    }
private:
    void free();
private:

    GLFWwindow* m_window;
    
    size_t m_width;
    size_t m_height;
    std::string m_title;
    WindowState m_state;
};

#endif // WINDOW_H