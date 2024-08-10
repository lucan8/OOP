#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>

class Textures{
private:
    GLuint id;
    std :: unique_ptr<GLubyte> localBuffer;
    GLint width, height, bpp;
public:
    Textures() : id(0){}
    //Loads the texture from the file path
    Textures(const std :: string& file_path);
    //Deep copy constructor
    Textures(const Textures& other);
    //Deep copy assignment operator
    Textures& operator=(const Textures& other);
    ~Textures();

    void bind(unsigned int slot) const;
    void unbind() const;

    GLuint get() const{return this->id;}
    //Returns the average color of the texture(not normalized, values from 0 to 255)
    glm :: vec4 getAverageColor() const;
};