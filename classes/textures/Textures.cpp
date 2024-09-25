#include "Textures.h"
#include "../../vendor/stb_image.h"
#include "../exceptions/MyRuntimeException.h"
#include "../constants/Constants.h"

Textures :: Textures(const std :: string& file_path): channel_count(Constants :: getVal("IMAGE_CHANNEL_COUNT")){
    load(file_path);
    __textures(GL_RGBA);
}


Textures :: Textures(const GLubyte* buffer, int width, int height)
    : localBuffer(std :: unique_ptr<GLubyte>((GLubyte*)buffer)), width(width), height(height), channel_count(Constants :: getVal("FONT_CHANNEL_COUNT")){
    __textures(GL_RED); 
};


void Textures :: __textures(GLint pixel_format){
    glGenTextures(1, &this->id);
    setParams();

    //Setting the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, this->width, this->height, 0, pixel_format, GL_UNSIGNED_BYTE, this->localBuffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}


Textures :: ~Textures(){
    glDeleteTextures(1, &this->id);
}


void Textures :: bind(uint16_t slot) const{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);
}


void Textures :: unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
}


glm :: vec4 Textures :: getAverageColor() const{
    glm :: vec4 avg_color(0.0f);
    for(int i = 0; i < this->width * this->height; i++){
        avg_color.r += this->localBuffer.get()[i * 4 + 0];
        avg_color.g += this->localBuffer.get()[i * 4 + 1];
        avg_color.b += this->localBuffer.get()[i * 4 + 2];
        avg_color.a += this->localBuffer.get()[i * 4 + 3];
    }
    avg_color /= this->width * this->height;
    return avg_color;
}


void Textures :: setParams() const{
    glBindTexture(GL_TEXTURE_2D, this->id);
    //Setting texture filtering options for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Setting texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


void Textures :: load(const std :: string& file_path){
    stbi_set_flip_vertically_on_load(1);
    
    this->localBuffer = std :: move(std :: unique_ptr<GLubyte>(stbi_load(file_path.c_str(), &width, &height, nullptr, 4)));
    if(this->localBuffer == nullptr)
        throw MyRuntimeException(__FILE__, __func__, __LINE__, "Failed to load texture from " + file_path);
}