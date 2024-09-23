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
    Font(){}
    // Font(Font&& other) : texture(std :: move(other.texture)), cdata(std :: move(other.cdata)){}
    // Font(const Font& other) : texture(other.texture), cdata(other.cdata){}
    // Font& operator=(Font other){
    //     swap(*this, other);
    //     return *this;
    // }
    // void swap(Font& first, Font& second){
    //     std :: swap(first.texture, second.texture);
    //     std :: swap(first.cdata, second.cdata);
    // }
    ~Font(){};
    glm :: mat4 getGlyphVertices(char c, glm :: vec2& pos) const;
    glm :: mat4 getProjMatrix() const;
    glm :: mat4 getScaleMatrix(float scale) const;
    //Binds the texture
    void bind() const {texture.bind();};
    //Returns the slot of the texture
    uint16_t getSlot() const {return texture.getSlot();};
    //Sets the uniforms for the font
    void setUniforms(Shader& shader, glm :: vec3 color, float scale) const;
};