#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IBO{
private:
    GLuint id;
public:
    IBO(GLuint* indices, GLsizeiptr size);
    void bind() const;
    void unbind() const;
    ~IBO();
};