#pragma once
#include "../../vendor/stb_truetype.h"
#include "../Textures/Textures.h"
#include <string>
//TODO: Add font sizes
class Font{
private:
    //unsigned char* buffer;
    Textures texture;
    std :: unique_ptr<stbtt_bakedchar> cdata;
public:
    Font(const std :: string& f_name);
    ~Font(){};
    // void bakeFont();
    // void bindTexture();
    // void unbindTexture();
    // void draw();
};