#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Textures{
private:
    GLuint id;
public:
    Textures() : id(0){}
    //Loads the texture from the file path
    Textures(const std :: string& file_path);
    Textures(const Textures& other) : id(other.id){}
    ~Textures();

    void bind(unsigned int slot) const;
    void unbind() const;

    GLuint get() const{return this->id;}
};