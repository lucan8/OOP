#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>
//TODO: Have options without mipmaps
//Slots are set during creation of the texture(will always be slots_in_use + 1)
class Textures;
typedef std :: unique_ptr<Textures> unique_texture;
class Textures{
private:
    GLuint id;
    std :: unique_ptr<GLubyte> localBuffer;
    GLint width, height, channel_count;

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
    //Two textures can't have the same id(if one gets deleted, the other will be invalid)
    Textures(const Textures& other) = delete;
    //Moving means we now have a texture with the same id as the other, which is invalid
    Textures(Textures&& other) = delete;
    
    Textures& operator=(const Textures& other) = delete;
    ~Textures();

    void bind(uint16_t slot) const;
    void unbind() const;

    GLuint get() const{return this->id;}
    //Returns the average color of the texture(not normalized, values from 0 to 255)
    glm :: vec4 getAverageColor() const;
};