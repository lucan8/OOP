#pragma once
#include "../VAO/VAO.h"
#include "../IBO/IBO.h"
#include "../shader/Shader.h"
#include "../font/Font.h"

class Renderer{
public:
    static void draw(const VAO& vao, const IBO& ibo, const Shader& shader);
    static void drawLine(const VAO& vao, const IBO& ibo, const Shader& shader);
    static void drawText(Shader& shader, const IBO& quad_ibo, const std :: string& text,
                         glm :: vec2 pos, const Font& font, float scale = 1, bool centered = false,
                         Font :: TextDirection text_dir = Font :: TextDirection :: RIGHT,
                         const glm :: vec3& color = glm :: vec3(1.0f));
private:
    Renderer(){};
    ~Renderer(){};
};