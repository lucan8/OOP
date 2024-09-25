#include "Font.h"
#include "../exceptions/FileOpenException.h"
#include "../../functions/functions.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
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
    this->texture = std :: make_unique<Textures>(bitmap, bitmap_width, bitmap_width);
}


glm :: mat4 Font :: getGlyphVertices(char c, glm :: vec2& pos, float scale) const{
    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(cdata.data(), Constants :: getVal("FONT_BITMAP_WIDTH"),
                       Constants :: getVal("FONT_BITMAP_WIDTH"), c - 32, &pos.x, &pos.y, &q, 1);
    
    scaleQuad(q, scale);
    //Updating the position for the next glyph
    pos.x += cdata[c - 32].xadvance * (scale - 1);

    return glm :: mat4( q.x0, q.y0, q.s0, q.t0,
                        q.x1, q.y0, q.s1, q.t0,
                        q.x1, q.y1, q.s1, q.t1,
                        q.x0, q.y1, q.s0, q.t1);

}


void Font :: setUniforms(Shader& shader, glm :: vec3 color, float scale) const{
    shader.bind();

    shader.setUniform3f("u_font_color", color);
    shader.setUniform1i("u_entity_type", Constants :: getEntityNumber("LETTER"));
    shader.setUniform1i("u_texture", 0);

    shader.setUniformMat4f("u_projection", Constants :: getPixelProj());
    //shader.setUniformMat4f("u_model", glm :: scale(glm :: mat4(1.0f), glm :: vec3(scale, scale, 1.0f)));
}


glm :: vec2  Font :: getTextStartPos(const std :: string& text, const glm :: vec2& pos, float scale) const{
    //The offset is half the sum of the xadvance of each character(the width of the character)
    float offset = 0;
    for (uint16_t i = 0; i < text.size(); ++i)
        offset += cdata[text[i] - 32].xadvance * scale;
    return pos - glm :: vec2(offset / 2, 0);
}


void Font :: scaleQuad(stbtt_aligned_quad& q, float scale) const{
    //Scaling the quad, but keeping the the middle point and the aspect ratio the same
    float half_width = (q.x1 - q.x0) / 2,
          half_height = (q.y1 - q.y0) / 2,
          middle_x = (q.x0 + q.x1) / 2,
          middle_y = (q.y0 + q.y1) / 2;

    q.x0 = middle_x - half_width * scale;
    q.x1 = middle_x + half_width * scale;
    q.y0 = middle_y - half_height * scale;
    q.y1 = middle_y + half_height * scale;
}


void Font :: bind(uint16_t slot) const{
    texture->bind(slot);
}