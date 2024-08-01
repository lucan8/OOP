#pragma once
#include <GL/glew.h>
#include "../textures/Textures.h"
class FBO{
private:
    GLuint fbo;
public:
    FBO();
    ~FBO();
    void bind();
    void bindTexture(const Textures& texture);
    
    void unbind();
};