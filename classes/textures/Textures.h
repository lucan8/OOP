#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>

//Slots are set during creation of the texture(will always be slots_in_use + 1)
class Textures{
private:
    GLuint id;
    int slot;
    std :: unique_ptr<GLubyte> localBuffer;
    GLint width, height, bpp;
    static uint16_t slots_in_use;

    //Sets and binds the texture to the slot
    void setSlot(int slot);
public:
    Textures() : id(0){}
    //Loads the texture from the file path
    Textures(const std :: string& file_path);
    //Deep copy constructor
    Textures(const Textures& other);
    //Deep copy assignment operator
    Textures& operator=(const Textures& other);
    ~Textures();

    void bind() const;
    void unbind();
    GLuint getSlot() const{return this->slot;}

    GLuint get() const{return this->id;}
    //Returns the average color of the texture(not normalized, values from 0 to 255)
    glm :: vec4 getAverageColor() const;
};