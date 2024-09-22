#include "Font.h"
#include "../exceptions/FileOpenException.h"
#include "../../functions/functions.h"
#include "../renderer/Renderer.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>

Font :: Font(const std :: string& file_path) {
    std :: string buffer = readFile(file_path, true);

    //Allocating memory for the font's bitmap (for the resolution, width and height are the same)
    uint16_t bitmap_width = Constants :: getVal("FONT_BITMAP_WIDTH");
    uint8_t* bitmap = new uint8_t[bitmap_width * bitmap_width];
    //Baking the font
    stbtt_BakeFontBitmap((uint8_t*)(buffer.c_str()), 0, 24, bitmap,
                         bitmap_width, bitmap_width,' ', cdata.size(), cdata.data());
                         
    //Creating the texture for that font
    this->texture = Textures(bitmap, bitmap_width, bitmap_width);
}


void Font :: draw(Shader& shader, const std :: string& text, glm :: vec2 pos, glm :: vec4 color){
    VertexBufferLayout layout;
    layout.addAttribute<float>(2);
    layout.addAttribute<float>(2);

    VBO vbo(4 * sizeof(float) * 4);
    VAO vao;
    vao.addBuffer(vbo, layout);
    vao.bind();

    IBO ibo(Constants :: getVertexIndices("SQUARE"), Constants :: getVal("NR_SQUARE_INDICES"));
    shader.bind();
    shader.setUniform1i("u_texture", texture.getSlot());
    shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("LETTER"));
    texture.bind();

    glm :: mat4 projection = glm :: ortho(0.0f, (float)Constants :: getVal("WINDOW_WIDTH"),
                                         (float)Constants :: getVal("WINDOW_HEIGHT"), 0.0f);
    shader.setUniformMat4f("u_projection", projection);

    uint16_t f_bitmap_width = Constants :: getVal("FONT_BITMAP_WIDTH");
    for (const auto& c : text){
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata.data(), f_bitmap_width, f_bitmap_width, c - 32, &pos.x, &pos.y, &q, 1);
        glm :: mat4 vertices = glm :: mat4( q.x0, q.y0, q.s0, q.t0,
                                            q.x1, q.y0, q.s1, q.t0,
                                            q.x1, q.y1, q.s1, q.t1,
                                            q.x0, q.y1, q.s0, q.t1);
        vbo.update(&vertices[0][0], sizeof(vertices));

        Renderer :: draw(vao, ibo, shader);
    }
}