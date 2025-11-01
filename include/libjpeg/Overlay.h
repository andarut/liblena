#ifndef OVERLAY_H
#define OVERLAY_H

#include "Utils.hpp"
#include "Logger.hpp"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Overlay {
public:
    Overlay();
    ~Overlay();
    int init(GLFWwindow* window);
    int render();

    int set_font(const std::string& pathToTtf, u8 fontSizeInPixels) {
        m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(pathToTtf.c_str(), fontSizeInPixels);
        if(!m_font) {
            return 1;
        }
        return 0;
    }
private:
    virtual void overlay();
    ImGuiContext* m_imguiCtx;
    ImFont* m_font;
};

#endif // OVERLAY_H