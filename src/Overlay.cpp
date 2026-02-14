#include "Overlay.h"
#include <imgui.h>

Overlay::Overlay()
    : m_imguiCtx(nullptr)
    {}

Overlay::~Overlay() {
    if (!m_imguiCtx) {
        ERROR("Overlay not initialized\n");
    } else {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(m_imguiCtx);
        m_imguiCtx = nullptr;
    }
}

int Overlay::init(GLFWwindow* window) {
    if (m_imguiCtx) {
        ERROR("Overlay already initialized\n");
        return 1;
    }

    m_imguiCtx = ImGui::CreateContext();
    if (!m_imguiCtx) {
        ERROR("Failed to create ImGui context\n");
        return 1;
    }
    
    ImGui::SetCurrentContext(m_imguiCtx);
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return 0;
}

void Overlay::overlay() {
    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Hello from Overlay!");
    ImGui::Text("This is an example overlay.");
    ImGui::End();
}

int Overlay::render() {
    if (!m_imguiCtx) {
        ERROR("Overlay not initialized\n");
        return 1;
    }

    ImGui::SetCurrentContext(m_imguiCtx);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImVec2 window_pos = ImVec2(0, 0);
    ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.0f);

    // bool show_demo_window = true;
    // ImGui::ShowMetricsWindow(&show_demo_window);

    if(m_font) {
        ImGui::PushFont(m_font);
    }
    overlay();
    if(m_font) {
        ImGui::PopFont();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return 0;
}