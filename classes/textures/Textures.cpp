#include "Textures.h"
#include "../../vendor/stb_image.h"
#include "../exceptions/MyRuntimeException.h"
#include "../constants/Constants.h"

uint16_t Textures :: slots_in_use = 0;

Textures :: Textures(const std :: string& file_path): channel_count(Constants :: getVal("IMAGE_CHANNEL_COUNT")){
    load(file_path);
    __textures(GL_RGBA);
}


Textures :: Textures(const GLubyte* buffer, int width, int height)
    : localBuffer(std :: unique_ptr<GLubyte>((GLubyte*)buffer)), width(width), height(height), channel_count(Constants :: getVal("FONT_CHANNEL_COUNT")){
    __textures(GL_RED); 
};

Textures :: Textures(const Textures& other): id(other.id), slot(other.slot), width(other.width),
                                             height(other.height), channel_count(other.channel_count){
    this->localBuffer = std :: unique_ptr<GLubyte>(new GLubyte[this->width * this->height * channel_count]);
    memcpy(this->localBuffer.get(), other.localBuffer.get(), this->width * this->height * channel_count);
}


Textures :: Textures(Textures&& other): id(other.id), slot(other.slot), width(other.width), channel_count(other.channel_count),
                                        localBuffer(std :: move(other.localBuffer)), height(other.height) {}


Textures& Textures :: operator=(Textures other){
    swap(*this, other);
    return *this;
}


void Textures :: __textures(GLint pixel_format){
    glGenTextures(1, &this->id);
    setParams();

    //Setting the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, this->width, this->height, 0, pixel_format, GL_UNSIGNED_BYTE, this->localBuffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    this->setSlot(Textures :: slots_in_use++);
    glBindTexture(GL_TEXTURE_2D, 0);

}


Textures :: ~Textures(){
    glDeleteTextures(1, &this->id);
    slots_in_use--;
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


void swap(Textures& first, Textures& second){
    using std :: swap;
    swap(first.id, second.id);
    swap(first.slot, second.slot);
    swap(first.width, second.width);
    swap(first.height, second.height);
    swap(first.channel_count, second.channel_count);
    swap(first.localBuffer, second.localBuffer);
}