#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "Utils.hpp"
#include "Logger.hpp"
#include "PPMReader.h"

#include "Window.h"
#include "Overlay.h"

class ImageView : public Window, public Overlay {
public:
    ImageView(const PPMImageData& image, const std::string& windowTitle, u8 pixelSize=1);

    int init();
    int render() override;

private:
    void overlay() override;
private:

    std::string m_windowTitle;

    // Данные изобрежния
    PPMImageData m_image;

    // Масштаб в пикселях
    u8 m_pixelSize;

    // Данные которые отображаем (RGB)
    std::vector<u8> m_data;

    GLuint m_texture = 0;
    GLuint m_vao = 0, m_vbo = 0;
    GLuint m_shaderProgram = 0;

    GLuint compileShader(GLenum type, const char* src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            std::cerr << "Shader compile error: " << log << "\n";
        }
        return shader;
    }
};

#endif // IMAGEVIEW_H