#include "Textures.h"
#include "../../vendor/stb_image.h"
Textures :: Textures(const std :: string& file_path){
    stbi_set_flip_vertically_on_load(1);
    this->localBuffer = std :: unique_ptr<GLubyte>(stbi_load(file_path.c_str(), &this->width, &this->height,
                                                             &this->bpp, 4));
    if(this->localBuffer == nullptr)
        throw std :: runtime_error("Failed to load texture from " + file_path);
    
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    //Setting the texture filtering option for magnification and minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Setting the texture wrapping option for S and T coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Setting the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->localBuffer.get());
    glBindTexture(GL_TEXTURE_2D, 0);
}


Textures :: ~Textures(){
    glDeleteTextures(1, &this->id);
}


void Textures :: bind(unsigned int slot) const{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);
}


void Textures :: unbind() const{
    glBindTexture(GL_TEXTURE_2D, 0);
}