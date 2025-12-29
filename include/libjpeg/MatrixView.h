#ifndef MATRIX_VIEW
#define MATRIX_VIEW

#include "Utils.hpp"
#include "Logger.hpp"
#include "PPMReader.h"

#include "Window.h"
#include "Overlay.h"
#include <format>
#include <imgui.h>
#include <vector>

/*
TODO
- отображение матрицы любого размера
- авто высчитывание отступом (через ImGui::CalcTextSize)

*/
template<typename T>
class MatrixView : public Window, public Overlay {
public:
    MatrixView(const std::array<T, 64>& data, u64 width, u64 height) : m_data(data), m_width(width), m_height(height) {}

    int init(const std::string& title) {
        if (Window::init(400, 400, title)) {
            ERROR("Failed to init window inside image view\n");
            return 1;
        }
        if (Overlay::init(getGLFWWindow())) {
            ERROR("Failed to init overlay inside image view\n");
            return 1;
        }
        set_font("../../Fonts/CascadiaMono/CaskaydiaMonoNerdFontPropo-Regular.ttf", 20);

        for(u64 i = 0; i < m_height; i++) {
            for(u64 j = 0; j < m_width; j++) {
                printf("%d ", m_data[i * m_height + j]);
            }
            printf("\n");
        }
        return 0;
    }

    int render() override {
        glfwMakeContextCurrent(getGLFWWindow());

        Window::render();
        
        glClear(GL_COLOR_BUFFER_BIT);

        Overlay::render();

        glfwSwapBuffers(getGLFWWindow());

        return 0;
    }

private:
    void overlay() override {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowBgAlpha(1);


        ImGui::Begin("Overlay", nullptr,
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_AlwaysAutoResize |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoFocusOnAppearing |
                    ImGuiWindowFlags_NoNav);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        draw_list->AddText(ImVec2(10,10), ImColor(255, 255, 255, 255), getTitle().c_str());

        u64 start_x = 30;
        u64 start_y = 60;

        u64 padding_size = 40;

        u64 cur_x = start_x;
        u64 cur_y = start_y;
        for(u64 i = 0; i < 8; i++) {
            for(u64 j = 0; j < 8; j++) {
                std::string num = std::format("{}", m_data[i * 8 + j]);
                draw_list->AddText(ImVec2(cur_x,cur_y), ImColor(255, 255, 255, 255), num.c_str());
                cur_x += padding_size;
            }
            cur_x = start_x;
            cur_y += padding_size;
        }

        ImGui::End();
    }
private:

    // Данные
    std::array<T, 64> m_data;

    u64 m_width;
    u64 m_height;
};

#endif // MATRIX_VIEW