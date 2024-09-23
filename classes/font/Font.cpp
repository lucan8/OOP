#include "Font.h"
#include "../exceptions/FileOpenException.h"
#include "../../functions/functions.h"
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
    stbtt_BakeFontBitmap((uint8_t*)(buffer.c_str()), 0, Constants :: getVal("FONT_SIZE"), bitmap,
                         bitmap_width, bitmap_width,' ', cdata.size(), cdata.data());
                         
    //Creating the texture for that font
    this->texture = Textures(bitmap, bitmap_width, bitmap_width);
}


glm :: mat4 Font :: getGlyphVertices(char c, glm :: vec2& pos) const{
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(cdata.data(), Constants :: getVal("FONT_BITMAP_WIDTH"),
                       Constants :: getVal("FONT_BITMAP_WIDTH"), c - 32, &pos.x, &pos.y, &q, 1);
    return glm :: mat4( q.x0, q.y0, q.s0, q.t0,
                        q.x1, q.y0, q.s1, q.t0,
                        q.x1, q.y1, q.s1, q.t1,
                        q.x0, q.y1, q.s0, q.t1);
}


void Font :: setUniforms(Shader& shader, glm :: vec3 color, float scale) const{
    shader.bind();

    shader.setUniform3f("u_font_color", color);
    shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("LETTER"));
    shader.setUniform1i("u_texture", this->getSlot());

    shader.setUniformMat4f("u_projection", getProjMatrix());
    shader.setUniformMat4f("u_model", getScaleMatrix(scale));
}


glm :: mat4 Font :: getProjMatrix() const{
    return glm :: ortho(0.0f, (float)Constants :: getVal("WINDOW_WIDTH"),
                        (float)Constants :: getVal("WINDOW_HEIGHT"), 0.0f);
}


glm :: mat4 Font :: getScaleMatrix(float scale) const{
    return glm :: scale(glm :: mat4(1.0f), glm :: vec3(scale, scale, 1.0f));
}