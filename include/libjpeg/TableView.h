#ifndef TABLE_VIEW
#define TABLE_VIEW

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
class TableView : public Window, public Overlay {
public:
    TableView(const std::vector<T>& data, u64 cols) : m_data(data), m_cols(cols) {}

    int init(const std::string& title) {
        if (Window::init(400, 400, title)) {
            ERROR("Failed to init window inside image view\n");
            return 1;
        }
        if (Overlay::init(getGLFWWindow())) {
            ERROR("Failed to init overlay inside image view\n");
            return 1;
        }
        set_font("../../Fonts/CascadiaMono/CaskaydiaMonoNerdFontPropo-Regular.ttf", 14);

        m_rows = std::round(static_cast<float>(m_data.size()) / static_cast<float>(m_cols));

        INFO("m_rows = %d\n", m_rows);

        INFO("m_cols = %d\n", m_cols);
        INFO("m_size = %d\n", m_data.size());
        for (u64 i = 0; i < m_data.size(); i++) {
            printf("%d ", m_data[i]);
        }
        printf("\n");

        for(u64 i = 0; i < m_rows; i++) {
            for(u64 j = 0; j < m_cols; j++) {
                u64 index = i * m_cols + j;
                if(index < m_data.size()) {
                    printf("%d ", m_data[index]);
                }
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
        u64 start_y = 130;

        u64 padding_size_x = 100;
        u64 padding_size_y = 40;

        draw_list->AddText(ImVec2(start_x,start_y-padding_size_y), ImColor(255, 255, 255, 255), "run");
        draw_list->AddText(ImVec2(start_x + padding_size_x,start_y-padding_size_y), ImColor(255, 255, 255, 255), "size");
        draw_list->AddText(ImVec2(start_x + padding_size_x * 2,start_y-padding_size_y), ImColor(255, 255, 255, 255), "ampl");


        u64 cur_x = start_x;
        u64 cur_y = start_y;
        for(u64 i = 0; i < m_rows; i++) {
            for(u64 j = 0; j < m_cols; j++) {
                u64 index = i * m_cols + j;
                if(index < m_data.size()) {
                    std::string num = std::format("{}", m_data[index]);
                    draw_list->AddText(ImVec2(cur_x,cur_y), ImColor(255, 255, 255, 255), num.c_str());
                    cur_x += padding_size_x;
                }
            }
            cur_x = start_x;
            cur_y += padding_size_y;
        }

        ImGui::End();
    }
private:

    // Данные
    std::vector<T> m_data;

    u64 m_cols;
    u64 m_rows;
};

#endif // TABLE_VIEW