#include "ImageView.h"
#include "ImageCh.h"
#include "PPMReader.h"
#include <imgui.h>

ImageView::ImageView(const PPMImageData& image, const std::string& windowTitle, u8 pixelSize) : m_image(image), m_windowTitle(windowTitle), m_pixelSize(pixelSize) {}

int ImageView::init() {

    u64 imageWidth = m_image[0].width();
    u64 imageHeight = m_image[0].height();

    u64 scaled_width = imageWidth * m_pixelSize;
    u64 scaled_height = imageHeight * m_pixelSize;

    if (Window::init(scaled_width, scaled_height, m_windowTitle.c_str())) {
        ERROR("Failed to init window inside image view\n");
        return 1;
    }
    if (Overlay::init(getGLFWWindow())) {
        ERROR("Failed to init overlay inside image view\n");
        return 1;
    }
    set_font("../../Fonts/CascadiaMono/CaskaydiaMonoNerdFontPropo-Regular.ttf", 14);

    m_data.resize(scaled_width * scaled_height * 4);
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            u8 r = m_image[0](y, x);
            u8 g = m_image[1](y, x);
            u8 b = m_image[2](y, x);
            u8 a = 255;

            for (int dy = 0; dy < m_pixelSize; ++dy) {
                for (int dx = 0; dx < m_pixelSize; ++dx) {
                    int dest_x = x * m_pixelSize + dx;
                    int dest_y = y * m_pixelSize + dy;
                    int dest_idx = (dest_y * scaled_width + dest_x) * 4;
                    m_data[dest_idx + 0] = r;
                    m_data[dest_idx + 1] = g;
                    m_data[dest_idx + 2] = b;
                    m_data[dest_idx + 3] = a;
                }
            }
        }
    }

    print("", m_image[0]);
    print("", m_image[1]);
    print("", m_image[2]);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, scaled_width, scaled_height,
                0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());
    float vertices[] = {
        // positions   // tex coords
        -1.f, -1.f,   0.f, 1.f,
        1.f, -1.f,   1.f, 1.f,
        1.f,  1.f,   1.f, 0.f,
        -1.f,  1.f,   0.f, 0.f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texcoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Shaders
    const char* vertSrc = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aTex;
    out vec2 TexCoord;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        TexCoord = aTex;
    })";

    const char* fragSrc = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;
    uniform sampler2D tex;
    void main() {
        FragColor = texture(tex, TexCoord);
    })";

    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vert);
    glAttachShader(m_shaderProgram, frag);
    glLinkProgram(m_shaderProgram);
    glDeleteShader(vert);
    glDeleteShader(frag);
    return 0;
}

void ImageView::overlay() {
    
    // Если в одном окне нужно будет переключаться между режимами
    // if (ImGui::BeginTabBar("Tabs")) {

    //     if (ImGui::BeginTabItem("Input")) {
    //         ImGui::Text("This is the Home tab.");
    //         ImGui::EndTabItem();
    //     }

    //     if (ImGui::BeginTabItem("DCT")) {
    //         ImGui::Text("This is the Settings tab.");
    //         ImGui::EndTabItem();
    //     }

    //     ImGui::EndTabBar();
    // }

    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    // bool isMouseInWindow = mousePos.x >= windowPos.x &&
    //                    mousePos.x <= windowPos.x + windowSize.x &&
    //                    mousePos.y >= windowPos.y &&
    //                    mousePos.y <= windowPos.y + windowSize.y;

    // if(!isMouseInWindow) {
    //     return;
    // }

    // ImVec2 mouse_local = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);

    // u64 pixel_i_cord = std::trunc((static_cast<float>(mouse_local.x) / static_cast<float>(m_pixelSize)));
    // u64 pixel_j_cord = std::trunc((static_cast<float>(mouse_local.y) / static_cast<float>(m_pixelSize)));

    // u64 pixel_i_center = pixel_i_cord * m_pixelSize;
    // u64 pixel_j_center = pixel_j_cord * m_pixelSize;
    
    // ImVec2 p_min(pixel_i_center, pixel_j_center);
    // ImVec2 p_max(pixel_i_center + m_pixelSize, pixel_j_center + m_pixelSize);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowBgAlpha(0.2f);


    ImGui::Begin("Overlay", nullptr,
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_AlwaysAutoResize |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing |
                 ImGuiWindowFlags_NoNav);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // ImGui::setw

    // ImGui::Text("X: %lu", pixel_i_cord);
    // ImGui::Text("Y: %lu", pixel_j_cord);
    // ImGui::Text("R: %d", m_image[0](pixel_j_cord, pixel_i_cord));
    // ImGui::Text("G: %d", m_image[1](pixel_j_cord, pixel_i_cord));
    // ImGui::Text("B: %d", m_image[2](pixel_j_cord, pixel_i_cord));

    // Draw RGB values on top of the every pixel

    u64 imageWidth = m_image[0].width();
    u64 imageHeight = m_image[0].height();

    u64 scaled_width = imageWidth * m_pixelSize;
    u64 scaled_height = imageHeight * m_pixelSize;

    for(u64 i = 0; i < scaled_height; i += m_pixelSize) {
        for(u64 j = 0; j < scaled_width; j += m_pixelSize) {
            u64 pixel_i_cord = j / m_pixelSize;
            u64 pixel_j_cord = i / m_pixelSize;

            u8 r = m_image[0](pixel_j_cord, pixel_i_cord);
            u8 g = m_image[1](pixel_j_cord, pixel_i_cord);
            u8 b = m_image[2](pixel_j_cord, pixel_i_cord);

            ImVec2 pixel_pos(j, i);
            char color_text[64];
            snprintf(color_text, sizeof(color_text), "R:%2d\nG:%2d\nB:%2d", r, g, b);
            // INFO("Drawing text at pixel (%llu, %llu): %s\n", pixel_i_cord, pixel_j_cord, color_text);
            // INFO("R = %d, G = %d, B = %d\n", r, g, b);
            draw_list->AddText(ImVec2(pixel_pos.x + 2, pixel_pos.y + 2), ImColor(255, 255, 255, 255), color_text);
        }
    }
    // draw_list->AddRect(p_min, p_max, IM_COL32(255, 0, 0, 255), 0.0f, 0, 3.0f);

    // TODO: draw pixel value text on pixel?
    // draw_list->AddText(ImVec2(pixel_i_center, pixel_j_center), ImColor(255, 255, 255, 255), "R");

    ImGui::End();
}

int ImageView::render() {

    // INFO("Rendering ImageView\n");
    
    glfwMakeContextCurrent(getGLFWWindow());

    Window::render();
    
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "tex"), 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    Overlay::render();

    glfwSwapBuffers(getGLFWWindow());

    return 0;
}