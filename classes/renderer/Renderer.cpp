#include "Renderer.h"

void Renderer :: draw(const VAO& vao, const IBO& ibo, const Shader& shader){
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer :: drawLine(const VAO& vao, const IBO& ibo, const Shader& shader){
    shader.bind();
    vao.bind();
    ibo.bind();
    glDrawElements(GL_LINE, ibo.getCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer :: drawText(Shader& shader, const IBO& quad_ibo, const std :: string& text, glm :: vec2 pos,
                          const Font& font, float scale, bool centered, Font :: TextDirection text_dir,
                          const glm :: vec3& color){
    font.setUniforms(shader, color, scale);
    font.bind(0);

    VertexBufferLayout layout;
    layout.addAttribute<float>(2);
    layout.addAttribute<float>(2);

    //4vertices, 2coords per position, 2coords per texture
    VBO vbo(sizeof(glm :: mat4));
    VAO vao;
    vao.addBuffer(vbo, layout);
    //If the text is centered we need to calculate the start position
    pos = centered ? font.getCenteredTextStartPos(text, pos, scale) : pos;
    for (const auto& c : text){
        glm :: mat4 vertices = font.getGlyphVertices(c, pos, scale, text_dir);
        vbo.update(&vertices[0][0], 4 * sizeof(float) * 4);
        Renderer :: draw(vao, quad_ibo, shader);
    }
}   