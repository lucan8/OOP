#pragma once
#include "../../vendor/stb_truetype.h"
#include "../textures/Textures.h"
#include "../shader/Shader.h"
#include "../constants/Constants.h"
#include <array>
#include <string>
//TODO: Add font sizes
class Font;
typedef std :: unique_ptr<Font> unique_font;
class Font{
private:
    unique_texture texture;
    //96 is the number of characters in the font
    std :: array<stbtt_bakedchar, 96> cdata;
    float max_letter_width = 0;
    float max_digit_width = 0;

    //Scale the width and height of the quad, but keep the middle point and the aspect ratio the same
    void scaleQuad(stbtt_aligned_quad& q, float scale) const;
    //Sets max_letter_width and max_digit_width
    void setMaxCharWidth();
public:
    Font(const std :: string& f_name);
    Font(){}
    Font(Font&& other) = delete;
    Font(const Font& other) = delete;
    Font& operator=(const Font& other) = delete;
        
    ~Font(){};
    //Returns the vertices of the glyph, and updates the position for the next glyph
    glm :: mat4 getGlyphVertices(char c, glm :: vec2& pos, float scale = 1.0) const;
    //Used for text that has half the chars on the left and half on the right of pos
    glm :: vec2 getCenteredTextStartPos(const std :: string& text, const glm :: vec2& pos, float scale = 1.0) const;
    //Binds the texture
    void bind(uint16_t slot) const;
    float getMaxLetterWidth() const{return max_letter_width;};
    float getMaxDigitWidth() const{return max_digit_width;};
    //Returns the slot of the texture
    //Sets the uniforms for the font
    void setUniforms(Shader& shader, glm :: vec3 color, float scale) const;
    static bool compCharWidth(const stbtt_bakedchar& a, const stbtt_bakedchar& b) {return a.xadvance < b.xadvance;}

    //Used for text that goes from right to left
    //glm :: vec2 getRightTextStartPos(const std :: string& text, const glm :: vec2& pos, float scale = 1.0) const;

};