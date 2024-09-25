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
public:
    Font(const std :: string& f_name);
    Font(){}
    Font(Font&& other) = delete;
    Font(const Font& other) = delete;
    Font& operator=(const Font& other) = delete;
        
    ~Font(){};
    glm :: mat4 getGlyphVertices(char c, glm :: vec2& pos) const;
    glm :: vec2 getTextStartPos(const std :: string& text, glm :: vec2& pos, float scale) const{
        pos.x -= text.size() * scale * Constants :: getVal("FONT_SIZE") / 2;
        pos.y -= scale * Constants :: getVal("FONT_SIZE") / 2;
        return pos;
    }
    //Binds the texture
    void bind(uint16_t slot) const;
    //Returns the slot of the texture
    //Sets the uniforms for the font
    void setUniforms(Shader& shader, glm :: vec3 color, float scale) const;

};