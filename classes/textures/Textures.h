#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>
//TODO: Have options without mipmaps
//Slots are set during creation of the texture(will always be slots_in_use + 1)
class Textures{
private:
    GLuint id;
    int slot;
    static uint16_t slots_in_use;
    std :: unique_ptr<GLubyte> localBuffer;
    GLint width, height, channel_count;

    //Sets and binds the texture to the slot
    void setSlot(int slot);
    void setParams() const;
    //Loads the texture from the file path(used for images)
    void load(const std :: string& file_path);
    void __textures(GLint pixel_format);
public:
    Textures() : id(0){}
    //Loads the texture from the file path
    Textures(const std :: string& file_path);
    //Creates a texture from a buffer(used for fonts)
    Textures(const GLubyte* buffer, int width, int height);   
    //Deep copy constructor
    Textures(const Textures& other);
    //Move constructor
    Textures(Textures&& other);
    //Can be used for deep copy and move
    Textures& operator=(Textures other);
    ~Textures();

    void bind() const;
    void unbind();
    GLuint getSlot() const{return this->slot;}

    GLuint get() const{return this->id;}
    //Returns the average color of the texture(not normalized, values from 0 to 255)
    glm :: vec4 getAverageColor() const;
    friend void swap(Textures& first, Textures& second);
};