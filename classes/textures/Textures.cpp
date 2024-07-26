#include "Textures.h"
#include "../../vendor/stb_image.h"
#include <memory>

Textures :: Textures(const std :: string& file_path){
    GLint width, height, bpp;
    stbi_set_flip_vertically_on_load(1);
    
    std :: unique_ptr<GLubyte> localBuffer(stbi_load(file_path.c_str(), &width, &height,&bpp, 4));
    if(localBuffer == nullptr)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer.get());
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