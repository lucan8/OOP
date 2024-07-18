#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

class Textures{
private:
    GLuint id;
    std :: unique_ptr<GLubyte> localBuffer;
    GLint width, height, bpp;
public:
    //Loads the texture from the file path
    Textures(const std :: string& file_path);
    ~Textures();

    void bind(unsigned int slot) const;
    void unbind() const;

    GLuint get() const{return this->id;}
    GLint getWidth() const{return this->width;}
    GLint getHeight() const{return this->height;}
    GLint getBPP() const{return this->bpp;}
    const std :: unique_ptr<GLubyte>& getLocalBuffer() const{return this->localBuffer;}
};