#pragma once
#include "../../vendor/stb_truetype.h"
#include "../textures/Textures.h"
#include "../shader/Shader.h"
#include "../constants/Constants.h"
#include <array>
#include <string>
//TODO: Add font sizes
class Font{
private:
    Textures texture;
    //96 is the number of characters in the font
    std :: array<stbtt_bakedchar, 96> cdata;
public:
    Font(const std :: string& f_name);
    ~Font(){};
    //Uses different projection matrix than the one used for rendering the match entities
    void draw(Shader& shader, const std :: string& text, glm :: vec2 pos, glm :: vec4 color = glm :: vec4(1.0f));
};