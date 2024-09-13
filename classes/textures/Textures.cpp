#include "Textures.h"
#include "../../vendor/stb_image.h"
#include "../exceptions/MyRuntimeException.h"

uint16_t Textures :: slots_in_use = 0;

Textures :: Textures(const std :: string& file_path){
    stbi_set_flip_vertically_on_load(1);
    
    this->localBuffer = std :: move(std :: unique_ptr<GLubyte>(stbi_load(file_path.c_str(), &width, &height,&bpp, 4)));
    if(this->localBuffer == nullptr)
        throw MyRuntimeException(__FILE__, __func__, __LINE__, "Failed to load texture from " + file_path);
    
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    //Setting the texture filtering option for magnification and minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Setting the texture wrapping option for S and T coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Setting the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->localBuffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    this->setSlot(Textures :: slots_in_use++);
    glBindTexture(GL_TEXTURE_2D, 0);
}


Textures :: Textures(const Textures& other) : id(other.id), width(other.width), height(other.height), bpp(other.bpp),
                                    localBuffer(std :: unique_ptr<GLubyte>(new GLubyte(other.height * other.width * 4))){
    memcpy(this->localBuffer.get(), other.localBuffer.get(), other.height * other.width * 4);
}


Textures& Textures :: operator=(const Textures& other){
    this->id = other.id;
    this->width = other.width;
    this->height = other.height;
    this->bpp = other.bpp;
    this->localBuffer = std :: unique_ptr<GLubyte>(new GLubyte(other.height * other.width * 4));
    memcpy(this->localBuffer.get(), other.localBuffer.get(), other.height * other.width * 4);
    return *this;
}


Textures :: ~Textures(){
    glDeleteTextures(1, &this->id);
}


void Textures :: bind() const{
    glActiveTexture(GL_TEXTURE0 + this->slot);
    glBindTexture(GL_TEXTURE_2D, this->id);
}


void Textures :: unbind(){
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


void Textures :: setSlot(int slot){
    if (slot >= 32)
        throw MyRuntimeException(__FILE__, __func__, __LINE__, "Too many textures loaded, max is 32");
    this->slot = slot;
}